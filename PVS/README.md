PVS DAIDALUS
==

[PVS version 6.0](http://pvs.csl.sri.com) and the development version
of the [NASA PVS Library](https://github.com/nasa/pvslib) are required
to proof-check the PVS DAIDALUS development. Furthermore, the directory
`PVS` has to be added to the Unix environment variable
`PVS_LIBRARY_PATH`.  Depending upon your shell, one of the following lines
has to be added to your startup script.  In C shell (csh or tcsh), put this line in
`~/.cshrc`, where `<wellclearpvsdir>` is the absolute path to the
directory `PVS`:

~~~
setenv PVS_LIBRARY_PATH "<wellclearpvsdir>:$PVS_LIBRARY_PATH"
~~~

In Borne shell (bash or sh), put this line in either `~/.bashrc or ~/.profile`:

~~~
export PVS_LIBRARY_PATH="<wellclearpvsdir>/nasalib:$PVS_LIBRARY_PATH"
~~~

To proof-check the PVS DAIDALUS development, type the following command in a Unix shell.

```
$ provethem all-theories
```

The output of that command is

```
TCASII                   [OK: 142 proofs]
WellClear                [OK: 243 proofs]
DAIDALUS                 [OK: 588 proofs]

*** Grand Total: 973 proofs / 973 formulas. Missed: 0 formulas.
```
