# cowsay
yet another cowsay clone that pretends to be better than any other ones.

## Usage
```-s``` pass text
```
$ cowsay -s hello
$ cowsay -s "hello, cruel world!"
```

```-e``` change eyes
```
$ cowsay -e @
```

```-w``` change output width for -s option
```
$ cowsay -w 40
P.S. 20 is default value
```

Also you can use pipes<br>
```
$ ls | cowsay
-----------------------------
| .git                       |
| .gitignore                 |
| .vs                        |
| cowsay.sln                 |
| cowsay.vcxproj             |
| cowsay.vcxproj.filters     |
| cowsay.vcxproj.user        |
| Release                    |
=============================
          \
           \
               ^__^
               (oo)_______
               (__)\       )\/\
                   ||----w |
                   ||     ||
```
