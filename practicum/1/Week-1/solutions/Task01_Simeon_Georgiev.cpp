#include <iostream>
#include <iomanip>
#include <cstring>
struct dict_t{
        char * word1;
        char * word2;
};
int replace(char * sentence, dict_t * d, unsigned dSize){
        for(unsigned i = 0; i < dSize; ++i){
                if ( strncmp(sentence, d[i].word1, strlen(d[i].word1)) == 0 ){
                        return i;
                }
        }
        return -1;
}
int main(){
        unsigned size;
        std::cin >> size;
        dict_t *d = new dict_t[size];
        for(unsigned i = 0; i < size; ++i){
                // std::cout << "i = " << i << '\n'; // commented out by Plamen
                unsigned wordLen;
                std::cin >> wordLen;
                std::cin >> std::setw(wordLen + 1);
                char * word1 = new char[wordLen + 1];
                std::cin >> word1;
                std::cin >> wordLen;
                std::cin >> std::setw(wordLen + 1);
                char * word2 = new char[wordLen + 1];
                std::cin >> word2;
                d[i].word1 = word1;
                d[i].word2 = word2;
        }
        unsigned sentenceLen;
        std::cin >> sentenceLen;
        std::cin >> std::ws;
        char * sentence = new char[sentenceLen + 1]; 
        std::cin.getline(sentence, sentenceLen + 1);
        unsigned sentenceOffset = 0;
        while(sentence[sentenceOffset] != '\0'){
                int replaceResult;
                if((replaceResult = replace(sentence + sentenceOffset, d, size)) != -1){
                        std::cout << d[replaceResult].word2;
                        sentenceOffset += strlen(d[replaceResult].word1);
                }else{
                        std::cout << sentence[sentenceOffset];
                        sentenceOffset++;
                }
        }

        // Added new line, by Plamen
        std::cout << '\n';

        for(unsigned i = 0; i < size; ++i){
                delete [] d[i].word1;
                delete [] d[i].word2;
        }
        delete [] sentence;
        delete [] d;
        return 0;
}
