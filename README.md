## PROJECT ##

* ID: **B**encode!**P**ARSER
* Contact: git@schepsen.eu

## USAGE ##

To use the **B**encode!**P**ARSER you need to include the **bdecoder.hpp** into your project,  e.g. #include "bdecoder.hpp"

## SPECIFICATION ##

* Bencode http://www.bittorrent.org/beps/bep_0003.html

## EXAMPLES ##

The string "**d3:bar4:spam3:fooi42ee**"
```
std::string src("d3:bar4:spam3:fooi42ee");
BencodeNode root
(
    BDecoder(src).decode()
);
```
will be encoded to a Json-like based bEncode structure
```
{ "bar" : "spam", "foo" : 42 }
```

## CHANGELOG ##

### Bencode!PARSER 1.1, updated @ 2016-03-22 ###

* Added the function encode() BEncodes a BencodeNode

### Bencode!PARSER 1.0, updated @ 2016-03-02 ###

* Added the function toString() prints formatted code after its decoding

### Bencode!PARSER 1.0, updated @ 2016-03-01 ###

* Initial Release
