# my-dwm-desktop-enviroment

## Useful commands
Creating symlinks for all resources in `scripts/`
```shell
for file in scripts/*; do sudo ln -sf `pwd`/$file /usr/local/bin/`basename "$file"` && chmod +x `pwd`/$file; done
```
My `/etc/fstab` looks like this
```shell
/swapfile none swap defaults 0 0 
tmpfs /tmp tmpfs rw,nodev,nosuid,size=22G 0 0 

//192.168.1.81/Pavel /home/pavtiger/nas cifs vers=3.0,credentials=/etc/nas-credentials,uid=1000,gid=1000,dir_mode=0755,file_mode=0755,soft
//192.168.1.81/Downloads /home/pavtiger/Videos/downloads cifs vers=3.0,credentials=/etc/nas-credentials,uid=1000,gid=1000,dir_mode=0755,file_mode=0755,soft
```
The last two lines are for mounting my home NAS. It sets correct permissions to access. The file `/etc/nas-credentials` is needed for passwords with file permissions **600** and owner **root** in this format:
```shell
username=<username>
password=<password>
domain=WORKGROUP
```

In dwmstatus I use emoji from [Nerdfonts](https://www.nerdfonts.com/cheat-sheet)

## Note
For wallpapergen script it is strongly recommended to use only 4K images if they are not the same aspect ratio as your screen. As this will lead to them being cropped and appearing very low-res and soapy.
