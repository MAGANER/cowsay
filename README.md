# cowsay
yet another cowsay clone that pretends to be better than any other clone

## Usage
```-f``` to pass file
```-t``` to pass any text as command line argument

Also you can use pipes

```
$ cowsay -f ..\.gitignore
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

```
λ l -f  | cowsay
---------------------------------------------------
| cowsay.Build.CppClean.log            |
| cowsay.exe                           |
| cowsay.exe.recipe                    |
| cowsay.iobj                          |
| cowsay.ipdb                          |
| cowsay.log                           |
| cowsay.pdb                           |
| cowsay.vcxproj.FileListAbsolute.txt  |
| main.obj                             |
| vc143.pdb                            |
===================================================
          \
           \
               ^__^
               (oo)_______
               (__)\       )\/\
                   ||----w |
                   ||     ||


```
