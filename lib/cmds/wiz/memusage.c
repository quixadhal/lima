/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CMD;

#define NUMBER 20

private void main(string str) {
    mapping mem = memory_summary();
    array tmp = allocate(NUMBER);
    int total = 0;

    if (str == "programs") {
        foreach (string prog, mapping elem in mem) {
            int subtotal = 0;
            int where = 0;
            
            foreach (string var, int size in elem)
                subtotal += size;
            total += subtotal;
                
            while (where < NUMBER && tmp[where] && tmp[where][1] > subtotal)
                where++;
            if (where == NUMBER) continue;
            tmp = tmp[0..where-1] + ({ ({ prog, subtotal }) }) + tmp[where..<2];
        }
        
        outf("Total object data: %d bytes\n\n", total);
        foreach (array entry in tmp)
            outf("%-60s %d bytes\n", entry[0], entry[1]);
        return;
    }
    
    if (str) mem = ([ str : mem[str] ]);
    
    foreach (string prog, mapping elem in mem) {
        foreach (string var, int size in elem) {
            int where = 0;
            
            total += size;
            while (where < NUMBER && tmp[where] && tmp[where][1] > size)
                where++;
            if (where == NUMBER) continue;
            tmp = tmp[0..where-1] + ({ ({ prog + ":" + var, size }) }) + tmp[where..<2];
        }
    }

    outf("Total object data: %d bytes\n\n", total);
    foreach (array entry in tmp) 
        outf("%-60s %d bytes\n", entry[0], entry[1]);
}
