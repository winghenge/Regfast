
----------
--.rgx File orginization
----------
The top part of the file comprises the preamble, and acts as a comment section.
This preamble will be copied as a multi-line comment to the output pattern-matcher, and is recomended to be used to provide an explintion of what is being matched. Is it for a language? email parsing? keyword spotting?


!! -- the double bangs represent the end of the preable and the start of the definition section. everything after this point will be treated as a regex:token pair, and '!!' ust be included in the .rgx file for any regex matcher to be generated.

! -- Single bangs in definition section are treated as single line comments
! -- they can be escaped for use in regex's using \

regex token -- a literal space cannot be inserted into a regex, it must be escaped, otherwise the first space that is enountered is treated as the seperation between regex and token. tokens can include literal spaces, but thats up to the end user.

----------
--Regular Expression Syntax
----------
[a-z] -> any lowercase
[A-Z] -> any uppercase
[0-9] -> any digit
[a-zA-Z] -> any alpha
[ ... ] -> set ex: [123] -> 1 or 2 or 3; [a-z0-9] -> a-z or 0-9
* -> any ammount
. -> wildcard
+ -> one-plus
^ -> not
\t -> tab
\  -> space
\s -> space
\. -> .
\+ -> +
\^ -> ^
\* -> *
\? -> !
\\ -> \
( ... ) -> group
\( \) -> ( )
... | ... -> ... or ...
\| -> |

