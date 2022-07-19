#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define
#define SEGWIT "0001"

struct in {
	
	char			txid[65];
	char			vin[9];
	char			*script_sig;
	char			sequence[8];
	int			len;
};

struct out {
	
	char			txid[64];
	char			*script_sig;
	char			sequence[8];
};

struct transaction {
	
	char			version[8];
	char			seg_mkrflag[4];
	char			inputs[2];
	struct inputs		*ins_arr;
	char			outputs[2];
	struct outputs		*outs_arr;
	char			locktime[8];
};


void sub_str(char *, char *, int, int);

int main(int argc, char *argv[])
{
	int			i, offset = 0;
	int			num_inputs, ss_len;
	char			c, *p, buffer[1000];
	struct transaction	tx;
	struct inputs		*inp;

        /* raw transaction is passed in thru argv[1],  I copy it to an char
           array called buffer, **fix_me: buffer should actually be delcared as
	   a char * instead of char arraay[1000] and I should Malloc()len of argv[1]).
	*/

	if (argv[1] == NULL) {
		printf("usage: raw tx arg requrired");
		exit(1);
	}

	strcpy(buffer,argv[1]);
	char *ptr = buffer;

        /* find script sig lengths for each input:
	the string starting at buffer plus 8 is the string representation of
        number of inputs, I need this value to calculate where the var_len
        fields are for each input. Need to convert it to an integer to make
        it useful. **fix_me: will need to account for fd,fe,ff to expand this
	value if necessary.
	*/

	char *tmp = malloc(3 * sizeof(char));
	strncpy(tmp, ptr += 10, 2);
	num_inputs = strtoul(tmp, &p, 16);


        /* there is one var len field for each input
           it is a string with 2 char's but I will need 3 chars
           in the new string to store it with a '\0' at the end.
	   so will need an array of strings each with 3 chars
	   
	 */

	
	/* first input */




        /*
          inp1 = starts at 10
              txid starts at + 0
              vout starts at + 64
              script sig start at + 72
              seq starts at + Plus len of scriptsig

          inp2 = starts at location of input 1 + lenofscriptsig1 + 80
          inp3 = starts at location of input 2 + lenfht of scriptsig 2 + 80
	  etc.....

	 */




	return 0;
}









/* random raw transaction
version 01000000   4  bytes  8 characters     0-7
segwit markerand flag 2 bytes 4 characters    8-11
inputs 01          1 byte    2 characters     either 8-9 or 12-13
txid 38966caadb834f37ddb3c51ec417dea9cd9c392e3ab465edd7959de6652c6b4e   32 bytes
64 characters  either 10-73 or 14-78
vin 00000000  
8b
scriptsig, long script sig, parse it out.
483045022100a66c833777f44b828bc1c327e4ee59fde31fd72b5577efabd85090d4ff8f9b6e022041921d797c592df2d566cc7f4fa119ada1aa3e7c8145c9543dd1e5ba765434c20141042ef60f0a969878a7d6acb4e8ba2645771c25b9713ceb4b545678bdfcc0507ab8b7c6650579c0f1b8ac09998501d372aa93657829c4f3c4a8268f0cf87aa8784c
seq; ffffffff
outputs 02
amount 1e5f990200000000
pubkey 1976a9149e71046a35c496d7035e57b564f680adb9a13e9688ac
amount 7100700000000000
pubkey 1976a914197807be6fab6fa0ccc6f0f6047a63b398b309f088ac
locktime 00000000 */


/* same as above unserialized 010000000138966caadb834f37ddb3c51ec417dea9cd9c392e3ab465edd7959de6652c6b4e000000008b483045022100a66c833777f44b828bc1c327e4ee59fde31fd72b5577efabd85090d4ff8f9b6e022041921d797c592df2d566cc7f4fa119ada1aa3e7c8145c9543dd1e5ba765434c20141042ef60f0a969878a7d6acb4e8ba2645771c25b9713ceb4b545678bdfcc0507ab8b7c6650579c0f1b8ac09998501d372aa93657829c4f3c4a8268f0cf87aa8784cffffffff021e5f9902000000001976a9149e71046a35c496d7035e57b564f680adb9a13e9688ac71007000000000001976a914197807be6fab6fa0ccc6f0f6047a63b398b309f088ac00000000
*/
/* catch errors, make sure the inpute string is of a certain lenght and only has legal characters */
/* we can determine start and end of every string that needs to be parsed, maybe
that should be all calculated first and placed in multi dimesnional array or
some other data structure instead of figure out as you gom
we need to start off by calculating the length of all the varibale length fields
I will only do the first byte now, I will take care of fd fe ff later, for more
complex
parsing.
also need to account for coinbase tx's.

*/


/*
field(index,len)

     if segwit (8,4) == TRUE index += 4;

version		(0,8)
#inputs_basic	(8,2)
txid		(10,64)
vin		(74,8)
ss		(82,var1)
seq		(82+var1,8)
EACH INPUTLENGHT IS  (80 + var1)

input 2 txid	(90  + var1, 64)
vin		(154 +var1, 8)
pss		(162+var1, var2)
seq		(162+var1+var2,8)

ouputs          (number of inputs *80 + var1,var2,var3...., 2)
amount		(outputs + 2, 16)

*/

/*
1. get number of inputs
2. where is the varlen field of the script sig for input 1
3. where is it for subsequent inputs ?


*/
