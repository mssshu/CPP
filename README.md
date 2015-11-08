# C++ program highlighting text using regex
</br>
<h5>description:</h5>
<p>
C++11 program to highlight lines of text using ANSI escape codes according to its < a-z > tags and will end the highlight at a closing tag < /a-z >. Nested tags are valid, for example:
</br></br>
< grey >this is a < blue >closed book < red >exam< /red >. 
</br>Read all questions< /blue >< purple >carefully< /purple >.< /grey >
</br></br>
"exam" will be in red, "closed book" and "read all questions" will be in blue.
</br></br>
To achieve this, the program will read one configuration file, store the contents in a map, then read from standard input and output according to the contents in the map.
</br></br>
The configuration file must contain two words on each line, any words after it is simply ignored. The first word is the name of a tag. The second word is the color code used for highlighting.
</br></br>
Test files are included.
</p>
</br>
<h5>conditions:</h5>
<p><ul>
	<li>tag names must be alphanumeric</li>
	<li>tags cannot be overlapped</li>
	<li>tags must match</li>
	<li>must contain < text > at the start with no characters before it except for whitespace (any whitespace will be deleted)</li>
	<li>must contain < /text > at the end with no characters after it except for whitespace (any whitespace will be deleted)</li>
	<li> cannot contain a "<" character without a matching ">" and vice versa</li>
	<li>entities "& l t ;" and "& g t ;" are changed to "<" and ">"</li>
	<li>some compilers accepts '\e' for the escape sequence, this is non-standard and should be avoided by replacing it to '\033'</li>
</p></ul>
</br>