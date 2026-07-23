# libpmdintf

A C library to interface with a running PC-98 PMD instance.

## About

PMD (Professional Music Driver) is an Assembly-written TSR program for playing music on various retro computer systems
in the background. It was developed by Masahiro Kajihara from 1989 to 2000, and is available on systems like:

- NEC PC-88 series
- NEC PC-98 series
- Fujitsu FM Towns
- Sharp X68000
- IBM PC

This project is a C library for interfacing with a running PC-98 PMD instance. It is based on the source-available
PC-98 version of PMD, available on KAJA's website.

## Building

You will need:

- OpenWatcom toolchain
  For targeting 16-bit x86 DOS. I use 1.9 since it's slop-free.
- GNU Make
  Because I could not be bothered to deal with Watcom WMake's syntax. Feel free to contribute a WMake file… :)

After making sure that you have set up the environment variables necessary for the OpenWatcom toolchain to target
16-bit DOS:

```sh
make -f GNUMakefile
```

## Contributing

No slop, go use your theft machine somewhere else please.

## License

The 30th anniversary release of the PC-98 PMD source code included the following statement by Masahiro Kajihara:

> ソースについての著作権は放棄しませんが、ご自由に使って頂いて構いません。
> むしろこんな古いものを今何か再利用するアイデアがあるようでしたら、ぜひ利用してやってください。

Translated:

> I don't relinquish copyright to the source code, but you're free to use it as you wish.
> In fact, if you have any ideas for reusing something this old, please feel free to do so.

I'm unsure if this constitutes as a "Public Domain" release (which is also extra murky since there are different
countries involved here), so I have treated the original source code as `Unfree, source available`.

The code in `/src` was written by reading the PMD source code, please consider that code to be
`Unfree, source-available` as well.

The code in `/examples` is licensed as `GPL-3.0-or-later`.
