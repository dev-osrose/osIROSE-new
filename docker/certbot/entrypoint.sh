#!/bin/sh
#
# Obtain and renew the deployment's TLS certificate over ACME DNS-01.
#
# DNS-01 is used rather than HTTP-01 because the game servers speak a custom
# binary protocol and there is no web server in the stack to answer a challenge
# on port 80.
#
# The certificate is published to $CERT_DIR under fixed, domain-agnostic names
# so that config/server.json can point at it without templating.

set -eu

log() { printf '[certbot-entrypoint] %s\n' "$*"; }
fail() { log "ERROR: $*"; exit 1; }

: "${ACME_DOMAIN:=}"
: "${ACME_EMAIL:=}"
: "${ACME_DNS_PLUGIN:=cloudflare}"
: "${ACME_DNS_PROPAGATION_SECONDS:=60}"
: "${ACME_RENEW_INTERVAL:=12h}"
: "${ACME_STAGING:=}"
: "${ACME_EXTRA_ARGS:=}"

CREDENTIALS_FILE=/secrets/dns.ini
DEPLOY_HOOK=/usr/local/bin/deploy-hook.sh

[ -n "$ACME_DOMAIN" ] || fail "ACME_DOMAIN is not set. Set it in .env."
[ -n "$ACME_EMAIL" ] || fail "ACME_EMAIL is not set. Set it in .env."

# ACME_DOMAIN may hold several space-separated names; they go on one
# certificate as SANs, and the first one names the lineage on disk.
LINEAGE=${ACME_DOMAIN%% *}
LIVE_DIR="/etc/letsencrypt/live/$LINEAGE"

domain_flags=""
for domain in $ACME_DOMAIN; do
  domain_flags="$domain_flags -d $domain"
done

# Plugin flags are assembled by name so swapping ACME_DNS_IMAGE to another
# certbot/dns-* image is the only change needed to switch providers.
plugin_flags="--dns-${ACME_DNS_PLUGIN}"
if [ -f "$CREDENTIALS_FILE" ]; then
  plugin_flags="$plugin_flags --dns-${ACME_DNS_PLUGIN}-credentials $CREDENTIALS_FILE"
else
  # route53 and a few others take no credentials file and read the environment.
  log "no $CREDENTIALS_FILE found; assuming the $ACME_DNS_PLUGIN plugin reads its credentials from the environment"
fi
plugin_flags="$plugin_flags --dns-${ACME_DNS_PLUGIN}-propagation-seconds $ACME_DNS_PROPAGATION_SECONDS"

case "$ACME_STAGING" in
  '' | 0 | false | no | off) ;;
  *)
    plugin_flags="$plugin_flags --staging"
    log "using the Let's Encrypt STAGING environment; certificates will not be trusted by clients"
    ;;
esac

if [ -s "$LIVE_DIR/fullchain.pem" ]; then
  log "certificate for $LINEAGE already present, skipping issuance"
else
  log "requesting a certificate for: $ACME_DOMAIN"
  # Deliberately unquoted: the flag strings must word-split into arguments.
  # shellcheck disable=SC2086
  certbot certonly \
    --non-interactive \
    --agree-tos \
    -m "$ACME_EMAIL" \
    --cert-name "$LINEAGE" \
    $domain_flags $plugin_flags $ACME_EXTRA_ARGS
fi

# `certbot renew` only fires --deploy-hook when it actually renews something, so
# the shared volume is seeded here instead: on a cold start, and again if the
# certs volume is ever wiped. The hook is a no-op when nothing changed, which
# keeps a plain restart of this container from bouncing the game servers.
RENEWED_LINEAGE="$LIVE_DIR" "$DEPLOY_HOOK"

trap 'log "signal received, shutting down"; exit 0' TERM INT

# Renewal reads the plugin and ACME server back from the lineage's renewal
# config, so the flags above must not be repeated here.
while :; do
  log "checking whether $LINEAGE is due for renewal"
  certbot renew --non-interactive --deploy-hook "$DEPLOY_HOOK" \
    || log "renewal attempt failed; will retry in $ACME_RENEW_INTERVAL"

  # Backgrounded so that a TERM during the wait is handled promptly.
  sleep "$ACME_RENEW_INTERVAL" &
  wait $! || true
done
