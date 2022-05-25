#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mainMenu();
void directoryReadToFiles();
void addToDirectoryTextFile();
void createHashTable();
void readFileConvertHash();
void readHashTable();

int hashTableSize();
int hashOneEncrypt();

char convertToStringHash[80];

int main() {
    mainMenu();
    return 0;
}


void mainMenu(){
    printf("********************************************\n");
    printf("*            Hashing Algoritması           *\n");
    printf("********************************************\n");
    printf("* 1-) Dizindeki dosyaları oku              *\n");
    printf("* 2-) Dizine yeni metin dosyası ekle       *\n");
    printf("* 3-) Dizin içeriğini şifrele              *\n");
    printf("* 4-) Hash Tablosunu oku                   *\n");
    printf("* 5-) Çıkış                                *\n");
    printf("********************************************\n");
    
    int secim = 0;
    printf("Seciminizi Giriniz : ");
    scanf("%d", &secim);
    switch (secim) {
        case 1: {
            directoryReadToFiles();
            printf("\n\n\n");
            mainMenu();
            break;
        }
        case 2: {
            addToDirectoryTextFile();
            printf("\n\n\n");
            mainMenu();
            break;
        }
        case 3: {
            readFileConvertHash();
            printf("\n\n\n");
            mainMenu();
            break;
        }
        case 4: {
            readHashTable();
            printf("\n\n\n");
            mainMenu();
            break;
        }
        case 5: {
            printf("Bye bye ..!\n");
            break;
        }
        default: {
            printf("Gecerli bir secim yapmadınız.");
            printf("\n\n\n");
            mainMenu();
            break;
        }
    }
}

void directoryReadToFiles(){
    DIR *dir;
    struct dirent *entry;
    FILE *fptr;
    char character;
    char content[100000];
    int length;

    dir = opendir("./Metinler");
    if (dir)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            length = strlen(entry->d_name);
            if (length > 4)
            {
                char filePath[] = "./Metinler/";
                strcat(filePath,entry->d_name);

                printf("\n%s\n",entry->d_name);

                fptr = fopen(filePath, "r");
		        if (fptr != NULL)
    		    {
                    character = fgetc(fptr);
                    while (character != EOF)
                    {
                        printf ("%c", character);
                        character = fgetc(fptr);
                    }
                    printf("\n");
                    fclose(fptr);
        	    }
            }
        }
        closedir(dir);
    }
}

void addToDirectoryTextFile(){
    FILE *file;
    DIR *dir;
    struct dirent *entry;
    char filePath[] = "./Metinler/";
    char text[500];
    char character;
    int length, size, hash1;
    int fileNumber = 1;
    char convertFileNumber[3];
    char stringHash1[3];

    dir = opendir("./Metinler");
    if (dir)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            length = strlen(entry->d_name);
            if (length > 4)
            {
                fileNumber++;
            }
        }
    }
    sprintf(convertFileNumber, "%d", fileNumber);

    strcat(filePath, convertFileNumber);
    strcat(filePath, ".txt");

    if ((file = fopen(filePath,"w")) == NULL){
        printf("Dosya oluşturulamadı.\n");
    }else{
        printf("\nDosya oluşturuldu...\nDosya içeriğine metin giriniz : ");
        scanf("\n");
        scanf("%[^\n]s", text);

        size = strlen(text);
        hash1 = hashOneEncrypt(text ,size);

        sprintf(stringHash1, "%d", hash1);
        char *find1 = strstr(convertToStringHash, stringHash1);

        if (find1 != NULL)
        {
            hash1 = 1 + (hash1 % (hashTableSize() - 1));
            sprintf(stringHash1, "%d", hash1);
            char *find2 = strstr(convertToStringHash, stringHash1);

            if (find2 != NULL)
            {   
                printf("Metin daha önce eklenmiş.\n");
            }else{
                strcat(convertToStringHash, stringHash1);
                strcat(convertToStringHash, " ");

                if ((fprintf(file, "%s\n", text)) != EOF)
                {
                    printf("Dosyaya içeriğine kaydedilen metin : %s\n ",text);
                    fclose(file);
                }else{
                    printf("Dosya içeriğine metin kaydedilemedi.\n");
                }
            }
        }else{
            if ((fprintf(file, "%s\n", text)) != EOF)
                {
                    printf("Dosyaya içeriğine kaydedilen metin : %s\n ",text);
                    fclose(file);
                }else{
                    printf("Dosya içeriğine metin kaydedilemedi.\n");
                }
        }
    }
}

void createHashTable(int hash1, char *fileName){
    FILE *file;
    char stringHash1[3];

    sprintf(stringHash1, "%d", hash1);
    char *find1 = strstr(convertToStringHash, stringHash1);

    if (find1 != NULL)
    {
        printf("Hash Tablosu birinci Hash değerini içeriyor.\n");
        hash1 = 1 + (hash1 % (hashTableSize() - 1));
        printf("İkinci Hash fonksiyonundan ikinci Hash değeri elde edildi.\n");
        sprintf(stringHash1, "%d", hash1);

        char *find2 = strstr(convertToStringHash, stringHash1);
        if (find2 == NULL)
        {
            strcat(convertToStringHash, stringHash1);
            strcat(convertToStringHash, " ");
            printf("Hash : %s -> Dosya Adı : %s\n",stringHash1, fileName);
            printf("Hash Tablosuna içerik kaydedildi.\n");
        }else{
            printf("Hash tablosunda yer bulunamadı.\n");
            printf("Hash Tablosuna içerik kaydedilemedi.\n");
        }
    }else{
        printf("Hash : %s -> Dosya Adı : %s\n",stringHash1, fileName);
        printf("Hash Tablosuna içerik kaydedildi.\n");
        strcat(convertToStringHash, stringHash1);
        strcat(convertToStringHash, " ");
    }
    
    if (((file = fopen("./hashWords.txt","a+")) == NULL)){
        printf("Hash Tablosu oluşturulamadı.");
    }else{
        if ((fprintf(file, "%s -> %s\n", stringHash1, fileName)) != EOF){
            fclose(file);
        }else{
            printf("Hash Tablosuna içerik kaydedilemedi.");
        }
    }
    printf("%s", convertToStringHash);
}

void readFileConvertHash() {
    DIR *dir;
    struct dirent *entry;
    FILE *fptr;
    char character;
    char content[100000];
    char filePath[] = "./Metinler/";
    int length, size, hash1;
    char text[1000];

    dir = opendir("./Metinler");
    if (dir)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            length = strlen(entry->d_name);
            if (length > 4)
            {
                char filePath[] = "./Metinler/";
                strcat(filePath,entry->d_name);

                printf("\n%s\n",entry->d_name);

                fptr = fopen(filePath, "r");
                
		        if (fptr != NULL){
                    fscanf(fptr, "%[^\n]s", text);
                    size = strlen(text);
                    hash1 = hashOneEncrypt(text, size);
                    createHashTable(hash1, entry->d_name);
                    printf("\n\n\n");
                    //printf("Hash of %s --> %d\n\n", text, hash); // kalkcak
                    fclose(fptr);
        	    }
            }
        }
        closedir(dir);
    }
}

void readHashTable(){
    FILE *fptr;
    char character;

    fptr = fopen("./hashWords.txt", "r");
    if ((fptr != NULL))
    {   
        printf("\n\n");
        character = fgetc(fptr);
        while (character != EOF)
        {
            printf ("%c", character);
            character = fgetc(fptr);
        }
        fclose(fptr);
    }else{
        printf("\nHash tablosu oluşturulmadı. Dosya mevcut değil.\n\n");
    }
}

int hashTableSize(){
    DIR *dir;
    struct dirent *entry;
    int length, tableSize;
    int totalNumberFiles = 0;

    dir = opendir("./Metinler/");
    if (dir)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            length = strlen(entry->d_name);
            if (length > 4) { totalNumberFiles++; }
        }
    }

    totalNumberFiles = (totalNumberFiles * 3);
    tableSize = totalNumberFiles;

    for (int i = 2; i < tableSize; i++){
        if (tableSize % i == 0)
        { i=2; tableSize--;}
    }
    return tableSize;
}

int hashOneEncrypt(const char* word, const int length){
    int hash = 0;

    long p_pow = 26;
    for(int i = 0; i < length; i++) {
        hash = (hash + (word[i] - 'a' + 1) * p_pow);
        hash = hash % hashTableSize();
    }
    return (hash < 0) ? hash * -1 : hash;
}

