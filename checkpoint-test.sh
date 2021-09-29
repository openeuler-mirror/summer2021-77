/bin/sh -c 'i=0; while true; do echo $i >a.txt; i=$(expr $i + 1); sleep 1; done'

isula run -d  --external-rootfs=/var/lib/isulad/storage/overlay/507395028f51f1c49facc13e19ac9e44d37d9ba2e2c5aab5ca1c8dd4b60065a0/merged --security-opt seccomp=unconfined ubuntu /bin/sh -c 'i=0; while true; do echo $i > a.txt; i=$(expr $i + 1); sleep 1; done'