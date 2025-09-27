# LightDM Hooks

Some very dirty hacks to improve LightDM user experience in Vlab.

- no-create-seat0: Prevent LightDM from creating seat0, which would fail and break the assumption of systemd session, making a lot of things subtly broken (and make podman rootless completely broken).
- xsessions-error-to-journald: Redirect X session error output to journald, so that it can be viewed with `journalctl` instead of being lost in `~/.xsession-errors`. Journald also provides much better log rotation and management -- there are several cases that `~/.xsession-errors` would grow very large and use up all disk space.

Note that these are dirty hacks, so they are not pre-installed in Vlab images by default.

```
docker build -t local/lightdm-hook-build .
docker run --rm -v $(pwd):/workspace local/lightdm-hook-build
```
