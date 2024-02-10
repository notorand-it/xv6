int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Trace mask must be specifed");
        exit(1);
    }

    /*
    get trace masks from args
    get program name and args
    fork process
      call trace(trace_mask) in child process
      exec program with programs args in the child process
    in parent, wait for child process to exit
    */
    char* trace_mask = argv[1];
    char* program = argv[2];
    char* program_args = argv[3];
}