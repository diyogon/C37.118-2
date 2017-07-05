#include <hammer/hammer.h>
#include <hammer/glue.h>
#include <stdio.h>

const HParser *C37_parser;

/*This holds all the parser rules, and is called first thing in main*/
void init_parser(){
  HParser *hex_digit = h_choice(h_ch_range('0', '9'), h_ch_range('a', 'f'), NULL);
  HParser *hex_octet = h_repeat_n(hex_digit, 2);
  H_RULE(sync, h_token("aa01", 4));
  H_RULE(prefix, h_sequence(h_many(hex_digit), sync, NULL));

  /*The final rule. Currently, a valid string must have:
   * 2 A's
   */
  H_RULE(tot, h_many(h_bits));
  //Now tell the main parser to use that rule to parse the input
  C37_parser = prefix;
}

int main(int argc, char *argv[]) {
    //Turn on the parser
    init_parser();

    //Build a buffer for the input and record its size
    uint8_t input[1024];
    size_t inputsize;
    inputsize = fread(input, 1, sizeof(input), stdin);

    //Debug: print that size to screen
    printf("%d\n", inputsize);
    //Run the parser. If it's valid, yay!, else boo!
    HParseResult *result = h_parse(C37_parser, input, inputsize);
    if(result) {
        printf("yay!\n");
    } else {
        printf("boo!\n");
    }
}