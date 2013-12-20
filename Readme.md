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

... which will generate _league.ttf.json_ and _league.ttf.png_ file.

You can also add some parameters for generation, for example:

```sh
$ fontatlas league.ttf size=512 padding=20
```

### Parameters

- __help__ - display usage
- __distmap__ - whether to generate as distmap or not (true by default)
- __size__ - size of the generated png (1024 by default)
- __resolution__ - size of the characters (50 by default)
- __padding__ - minimal space between the characters (20 by default)
- __output_dir__ - target directory of the generated file ("." by default)
