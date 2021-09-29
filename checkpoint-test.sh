/bin/sh -c 'i=0; while true; do echo $i >a.txt; i=$(expr $i + 1); sleep 1; done'

isula run -d  --external-rootfs=/var/lib/isulad/storage/overlay/8f181e47046ea672f0432ff987b15e64c4a399986e5b04e5feea1b313ca68943/merged --security-opt seccomp=unconfined busybox /bin/sh -c 'i=0; while true; do echo $i; i=$(expr $i + 1); sleep 1; done'

lxc-checkpoint -r -n a638b560bdfb580032f54c2041b62e92fc52a909612cd2af3ef837529b4525be -D /tmp/criu/test1 -P /var/lib/isulad/engines/lcr