#include "sor.h"
#include "flags.h"
#include <limits.h>

using namespace std;

int main(int argc, char** argv) {
    // set up a configuration object
    Flags flags = Flags(argc, argv);

    SorAdapter * sorAdapter = new SorAdapter(flags.from, flags.len, flags.filename);
    //if -print_col_type input was valid
    if (flags.printColType != UINT_MAX) {
        sorAdapter->printColumnType(flags.printColType);
    }
    
    //if -print_col_idx input was valid
    if (flags.printCell) {
        sorAdapter->printValue(flags.printColIndexCol, flags.printColIndexOffset);
    } 

    //if -is_missing_idx input was valid
    if (flags.printIsMissing) {
        sorAdapter->printIsMissing(flags.isMissingCol, flags.isMissingOffset);
    }

    delete sorAdapter;

    return 0;
}