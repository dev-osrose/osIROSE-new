#!/bin/sh
#
# certbot --deploy-hook: publish the freshly issued material to the shared certs
# volume and bounce the game servers.
#
# The restart is not optional. The servers load the certificate once, while
# building the asio SSL context at listener setup (see rose_ssl_config.h), and
# there is no reload path - no SIGHUP handler, and enable_ssl_client() refuses
# to reconfigure a socket that is already connected.
#
# certbot exports RENEWED_LINEAGE (the live/<name> directory) to deploy hooks;
# entrypoint.sh sets it by hand when seeding the volume on a cold start.

set -eu

log() { printf '[certbot-deploy-hook] %s\n' "$*"; }

: "${RENEWED_LINEAGE:?RENEWED_LINEAGE is not set - this script runs as a certbot deploy hook}"
: "${ACME_RESTART_CONTAINERS:=osirose-login osirose-character osirose-map}"
: "${ACME_RESTART_TIMEOUT:=90}"

CERT_DIR=/certs

# Wait for a restarted container to report healthy again, so that the next one
# in the chain finds its dependency listening. Gives up rather than blocking
# certbot forever; the container's own restart policy covers the rest.
wait_healthy() {
  _name=$1
  _waited=0
  while [ "$_waited" -lt "$ACME_RESTART_TIMEOUT" ]; do
    _status=$(docker inspect -f '{{if .State.Health}}{{.State.Health.Status}}{{else}}none{{end}}' "$_name" 2>/dev/null || echo missing)
    case "$_status" in
      healthy) log "$_name is healthy again"; return 0 ;;
      none) log "$_name has no healthcheck, not waiting"; return 0 ;;
      missing) return 1 ;;
    esac
    sleep 2
    _waited=$((_waited + 2))
  done
  log "WARNING: $_name did not become healthy within ${ACME_RESTART_TIMEOUT}s, continuing anyway"
  return 0
}

# Seeding runs on every start of the certbot container, so compare first: an
# unchanged certificate must not turn a routine restart into a stack-wide bounce.
if cmp -s "$RENEWED_LINEAGE/fullchain.pem" "$CERT_DIR/fullchain.pem"; then
  log "certificate in $CERT_DIR is already current, nothing to do"
  exit 0
fi

log "publishing $RENEWED_LINEAGE to $CERT_DIR"
install -m 0644 "$RENEWED_LINEAGE/fullchain.pem" "$CERT_DIR/fullchain.pem"
install -m 0600 "$RENEWED_LINEAGE/privkey.pem" "$CERT_DIR/privkey.pem"

# Ordered, not parallel: character opens its ISC client to login at startup and
# map opens one to character, so each dependency must be listening again before
# the next is bounced. Failures are tolerated - on the very first issuance these
# containers do not exist yet, because they wait on this one becoming healthy.
for container in $ACME_RESTART_CONTAINERS; do
  if docker restart "$container" >/dev/null 2>&1; then
    log "restarted $container"
    wait_healthy "$container" || log "$container disappeared while waiting"
  else
    log "could not restart $container (not running yet?)"
  fi
done

log "done"
