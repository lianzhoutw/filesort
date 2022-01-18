# filesort
Sort a big file which can't be loaded into memory  
  Usage: filesort [OPTIONS] FILE  
  -h,--help     Display this help and exit.  
  -o,--out-file Output file which stores the sorting results  
  -t,--test-num Number of test content, default is 1000000  
  -c,--cmd              Command to execute  
      - sort (default)       Sort the FILE  
      - gen_test             Generate test content to FILE  
