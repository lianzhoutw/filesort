# filesort
Sort a big file which can't be loaded into memory  
Usage: filesort [OPTIONS] FILE  
* -h,--help     Display this help and exit.  
* -o,--out-file Output file which stores the sorting results  
* -t,--test-num Number of test content, default is 1000000  
* -s,--seg-size Size in bytes of each block file
* -c,--cmd              Command to execute  
  * -sort (default)       Sort the FILE  
  * -gen_test             Generate test content to FILE  

# Example
* Generate a test file contains 1000000 lines of numbers .  
   filesort -t1000000 -c gen_test /tmp/filesort/in.txt 
* Sort the file in.txt and write the result into out.txt .  
  filesort -o /tmp/filesort/out.txt /tmp/filesort/in.txt
