Jaxen Bujold - 2
Christopher Esquibel - 2

Answer to question posed in lab 9 manual:
In real life, this example would need a bit more structure to work as part of a full k0 compiler. First, function calls need to 
follow a clear calling convention, especially something like printf, where the format string has to come before the values being printed. 
More importantly for k0, the println with curly-brace variables like "Variable i is $i", can’t just exist as-is. The string would need to be translated 
into something like a format string plus arguments, such as "Variable i is %d" with i passed separately. Secondly, The way locals and temporaries 
are handled here with fixed offsets (loc:0, loc:8, etc.) is fine for a toy, but in k0 this should come from our symbol table and type system 
so sizes and alignment are correct. Third, the string section is also simplified. realistically, we will need to track actual string sizes and give them 
proper labels instead of assuming fixed slots. On top of that, temporaries should be generated automatically, not hardcoded, 
and the TAC should probably carry type information so later stages will not have to guess.