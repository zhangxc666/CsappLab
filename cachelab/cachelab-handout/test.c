#include <stdio.h>
    int a[16][16]={0},b[16][16];
int main(){
    int cnt=0;
    for(int i=0;i<16;i++)
        for(int j=0;j<16;j++)a[i][j]=++cnt;
            printf("\n");
    for(int i=0;i<16;i+=8){
        for(int j=0;j<16;j+=8){
            for(int ii=0;ii<4;ii++){
                int temp[8]={0};
                for(int jj=0;jj<8;jj++)temp[jj]=a[i+ii][j+jj];
                for(int jj=0;jj<4;jj++)b[j+jj][ii+i]=temp[jj],b[j+jj][ii+4+i]=temp[jj+4];
            }
            for(int ii=0;ii<4;ii++){
                    int temp[8]={0};
                    for(int jj=0;jj<8;jj++)temp[jj]=a[i+ii+4][j+jj];
                    for(int jj=0;jj<4;jj++)b[j+jj+4][ii+i]=temp[jj],b[j+jj+4][ii+4+i]=temp[jj+4];
                }
            for(int ii=0;ii<4;ii++){
                int temp[8]={0};
                for(int jj=0;jj<4;jj++){
                    temp[jj]=b[j+ii][i+jj+4];
                }
                for(int jj=0;jj<4;jj++){
                    temp[jj+4]=b[j+ii+4][i+jj];
                }
                for(int jj=0;jj<4;jj++){
                    b[j+ii][i+jj+4]=temp[jj+4];
                }
                for(int jj=0;jj<4;jj++){
                    b[j+ii+4][i+jj]=temp[jj];
                }
            }
        }
    }
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            printf("%d\t",a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            printf("%d\t",b[i][j]);
        }
        printf("\n");
    }
}