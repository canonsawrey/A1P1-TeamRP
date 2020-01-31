For part 1, we decided to implement the SoR processor in C++, due to 
its high performance in comparison to other languages, ease to setup
with Docker, and availability of libraries such as vector.h and string.h. 

Processing was done by parsing the file character by character, using
switch statements on the ASCII value of each character, and using local
variables to hold the state of the SoR. 

The SoR file was both read in to a 2D vector, of type string, and the 
type of each column determined when reading from the file. Then, the 
2D vector of strings was used to construct a vector of 'Column'.

Column is a parent type to 'TypeColumn'. TypeColumn is a template class, 
that receives the C++ datatype equivalent to the defined types of BOOL,
STRING, INT, and FLOAT (bool, string, unsigned int, float). TypeColumn 
holds both the raw string and the converted data type of the SoR data, 
as well as the 'Type' of column it is. 

The reason for using a parent type of Column is because C++ would 
not allow the use of 'vector<TypeColumn>' without specifying the template
type. Using a non-template parent class Column allowed us to get around this. 

One other interesting part of our implementation was the use of the local
variable 'stagingVector' of type vector<string> when parsing the SoR file. 
The stagingVector held all of the strings for a line, and then placed those 
strings into the 2D vector of data after hitting a newline. This is because
we did not want to add a row until we knew the entire row would be read due to
the possibilty of a "length" stipulation. If reading of the file finishes 
not due to the length requirement, the final stagingVector is added regardless. 