# my-dwm-desktop-enviroment

## Useful commands
Creating symlinks for all resources in `scripts/`
```shell
for file in scripts/*; do sudo ln -sf `pwd`/$file /usr/local/bin/`basename "$file"` && chmod +x `pwd`/$file; done
```

## Note
For wallpapergen script it is strongly recommended to use only 4K images if they are not the same aspect ratio as your screen. As this will lead to them being cropped and appearing very low-res and soapy.
