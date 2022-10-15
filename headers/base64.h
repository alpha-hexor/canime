#include <iostream>

using namespace std;


char *base64encode(string data){
    //base64 encoding
    char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    //result string size
    char *res = (char *)malloc(1000*sizeof(char));

    int index,no_of_bits=0,padding=0,val=0,count=0,temp,i,j,k=0;

    int len_str = data.size();

    for(i =0;i<len_str;i+=3){
        val =0,count=0,no_of_bits=0;


        for(j=i;j<len_str && j<=i+2;j++){
            val = val << 8;
            val = val | data[j];
            count ++;

        }
        
        
    
        no_of_bits = count*8;
        padding = no_of_bits%3;

        while(no_of_bits!=0){
            if(no_of_bits>=6){
                temp = no_of_bits -6;

                index = (val >> temp) & 63;
                no_of_bits -=6;
            }

            else{
                temp = 6- no_of_bits;
                index = (val << temp) & 63;
                no_of_bits = 0;
            }
            res[k++] = char_set[index];
        }
    }

    //padding
    for(i=1;i<=padding;i++){
        res[k++] = '=';
    }
    res[k] = '\0';

    return res;
}

//function to decode base64
char *base64decode(string data){
    //base64 decoding

    
    char *result = (char *)malloc(1000000*sizeof(char));

    int i,j,k=0,num=0,count_bits=0;

    for(i=0;i<data.size();i+=4){
        num=0,count_bits=0;
        for(j=0;j<4;j++){
            if(data[i+j] != '='){
                num = num << 6;
                count_bits +=6;
            }

            if (data[i + j] >= 'A' && data[i + j] <= 'Z')
                num = num | (data[i + j] - 'A');
            else if (data[i + j] >= 'a' && data[i + j] <= 'z')
                num = num | (data[i + j] - 'a' + 26);
 
            
            else if (data[i + j] >= '0' && data[i + j] <= '9')
                num = num | (data[i + j] - '0' + 52);
 
            // '+' occurs in 62nd position in char_set.
            else if (data[i + j] == '+')
                num = num | 62;
 
            // '/' occurs in 63rd position in char_set.
            else if (data[i + j] == '/')
                num = num | 63;
            
            else{
                num = num >> 2;
                count_bits -=2;
            }
        }

        while(count_bits !=0){
            count_bits -= 8;
            result[k++] = (num >> count_bits) & 255;
        }
    }

    result[k] = '\0';

    return result;
}
