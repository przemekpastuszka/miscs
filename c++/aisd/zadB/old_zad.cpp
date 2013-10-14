#include <cstdio>
#include <iostream>

//knights_to_mask[i] = j <- ustawienie skoczków kodowane przez i generuje maskę kodowaną przez j
//-1 jeśli ustawienie i jest niepoprawne
int knights_to_mask[] = {0, 2, 5, 7, 2, 2, 7, 7, 20, 22, 21, 23, -1, -1, -1, -1, 40, 42, 45, 47, 42, 42, 47, 47, 60, 62, 61, 63, -1, -1, -1, -1, 17, -1, 21, -1, 19, -1, 23, -1, 21, -1, 21, -1, -1, -1, -1, -1, 57, -1, 61, -1, 59, -1, 63, -1, 61, -1, 61, -1, -1, -1, -1, -1};
//liczba skoczków w ustawieniu
int knights_number[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6};
int masks[] = {0, 32, 16, 8, 4, 2, 1};
//maski w kolejności od największej liczby jedynek do najmniejszej liczby jedynek
int masks_order[] = {63, 62, 61, 59, 55, 47, 31, 60, 58, 57, 54, 53, 51, 46, 45, 43, 39, 30, 29, 27, 23, 15, 56, 52, 50, 49, 44, 42, 41, 38, 37, 35, 28, 26, 25, 22, 21, 19, 14, 13, 11, 7, 48, 40, 36, 34, 33, 24, 20, 18, 17, 12, 10, 9, 6, 5, 3, 32, 16, 8, 4, 2, 1, 0};

int main(){
    
    //ile można ustawić pionków jeśli zdecydujemy się na dane ułożenie skoczków w dwóch ostatnich wierszach
    int knights_values[64]; 
    int mask_value[64]; //ile można max. ustawić pionków przy takiej masce 
    for(int i = 0; i < 64; ++i){
        mask_value[i] = 0;
    }
    
    int n;
    std :: cin >> n;
    //scanf("%d\n", &n);
    
    //std :: cout << n << std :: endl;
    
    for(int z = 0; z < n; z += 2){
        //INPUT
        char input[7];
        int curr_mask = 0; //wczytana maska
        //scanf("%c%c%c\n%c%c%c", input, input + 1, input + 2, input + 3, input + 4, input + 5);
        //scanf("\n");
        
       if(n - 1 > z){
            std :: cin >> input >> input + 3;
            //fgets(input, 5, stdin);
            //printf("kupa: %s", input);
            //fgets(&input[3], 5, stdin);
        }
        else{
            std :: cin >> input;
            //fgets(input, 5, stdin);
            input[3] = input[4] = input[5] = 'x';
        }
        
        for(int i = 0; i < 6; ++i){
            if(input[i] == 'x'){
                curr_mask |= masks[i + 1];
            }
        }
        //printf("%c%c%c\n%c%c%c", input[0], input[1], input[2], input[3], input[4], input[5]);
        //printf("MASK %d\n", curr_mask);
        
        //OBLICZ MAX. LICZBĘ SKOCZKÓW PRZY WSZYSTKICH MOŻLIWYCH ICH USTAWIENIACH NA 2 DOLNYCH WIERSZACH PLANSZY
        for(int i = 0; i < 64; ++i){
            //jeśli ustawienie jest poprawne i pozwala na nie wczytana maska
            if(knights_to_mask[i] >= 0 && ((curr_mask & i) == 0)){
                //możemy ustawić nowe pionki (knight_number) + max. ile da się wyżej przy generowanej masce
                knights_values[i] = knights_number[i] + mask_value[knights_to_mask[i]];
            }
            else{
                knights_values[i] = 0;
            }
            /*printf("%d, ", knights_values[i]);
            if(knights_values[i] == 6){
                printf("LOL %d\n", i);
            }*/
        }
        //printf("\n");
        
        
        //OBLICZ NOWE MASKI
        for(int i = 0; i < 64; ++i){
            int m = masks_order[i];
            mask_value[m] = knights_values[(~m) & 63];
            for(int j = 1; j <= 6; ++j){
                if(mask_value[m | masks[j]] > mask_value[m]){
                    mask_value[m] = mask_value[m | masks[j]];
                }
            }
        }
        
        /*for(int i = 0; i < 64; ++i){
            printf("%d, ", mask_value[i]);
        }
        printf("\n");*/
    }
    printf("%d\n", mask_value[0]);
    //std :: cout << mask_value[0] << std :: endl;
    
    return 0;
}
