# my-dwm-desktop-enviroment

Creating symlinks for all resousrces in `scripts/`
```shell
for file in scripts/*; do sudo ln -sf `pwd`/$file /usr/local/bin/`basename "$file"`; done
```

