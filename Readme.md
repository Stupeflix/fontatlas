FontAtlas
==========

Install
-------

```sh
$ mkdir build && cd build && cmake .. && make && sudo make install
```

Usage
-----

You can generate your font atlas and your metadata simply by using...

```sh
$ fontatlas league.ttf
````

... which will generate league.ttf.json and league.ttf.png file.

You can also add some parameters for generation, for example:

```sh
$ fontatlas league.ttf size=512 padding=20
```

### Parameters

- __distmap__ - whether to generate as distmap or not
- __size__ - size of the generated png
- __resolution__ - size of the characters
- __padding__ - minimal space between the characters
- __output_dir__ - target directory of the generated file
