# The DNS plugin is chosen by swapping the base image, so any certbot/dns-*
# image works without touching this file. Override with ACME_DNS_IMAGE.
ARG ACME_DNS_IMAGE=certbot/dns-cloudflare:latest
FROM ${ACME_DNS_IMAGE}

# Upstream certbot images are Alpine based. The deploy hook restarts the game
# containers through the mounted docker socket, which needs the client.
RUN apk add --no-cache docker-cli

COPY entrypoint.sh deploy-hook.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/entrypoint.sh /usr/local/bin/deploy-hook.sh

# The base image sets ENTRYPOINT ["certbot"]; we drive certbot ourselves.
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
