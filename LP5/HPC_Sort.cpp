#include<vector> 
#include<iostream>
#include<omp.h>
#include<chrono>
using namespace std;
using namespace std::chrono;
void bubble(vector<int> &arr){
    int n=arr.size();
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}
void pbubble(vector<int> &arr){
    int n=arr.size();
   #pragma omp parallel for
    for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
        if (arr[j] > arr[j + 1]) {
            swap(arr[j], arr[j + 1]);
        }
    }
}

}
void merge(vector<int> &arr,int left,int right,int mid ){
    int n1=mid-left+1;
    int n2=right-mid;
    vector<int> L(n1);
     vector<int> R(n2);

    for(int i=0;i<n1;i++){
        L[i]=arr[left+i];
    }
     for(int i=0;i<n2;i++){
        R[i]=arr[mid+i+1];
    }
    int i=0;int j=0;int k=left;
    while(i<n1 && j<n2){
        if(L[i]<R[j]){
            arr[k++]=L[i++];
        }else{
            arr[k++]=R[j++];
        }
    }
    while(i<n1){
        arr[k++]=L[i++];
    }
    while(j<n2){
        arr[k++]=R[j++];
    }
}
void merge_sort(vector<int> &arr,int left,int right){
    if(left<right){
        int mid=left+(right-left)/2;
        merge_sort(arr,left,mid);
        merge_sort(arr,mid+1,right);
        merge(arr,left,right,mid);
    }
}
void merge_sort_p(vector<int> &arr,int left,int right){
    
    if(left<right){
        
        int mid=left+(right-left)/2;
        #pragma omp parallel sections
        {
        #pragma omp section
        merge_sort(arr,left,mid);
        #pragma omp section
        merge_sort(arr,mid+1,right);
        
        }
        
        merge(arr,left,right,mid);
    }
}
int main(){
    int n=10000;
    vector<int> arr;
    for(int i=0;i<n;i++){
        arr.push_back(rand()%n);
    }
   // double start=omp_get_wtime();
   auto start=high_resolution_clock::now();
    bubble(arr);
   // double end=omp_get_wtime();
   auto end=high_resolution_clock::now();
    double overall_seq=duration<double,nano>(end-start).count();
    cout<<"Sequential one "<< overall_seq<<"\n";
    
    vector<int> arr2=arr;
    //  start=omp_get_wtime();
     start=high_resolution_clock::now();
    pbubble(arr2);
    //end=omp_get_wtime();
      end=high_resolution_clock::now();
   // double overall_parr=end-start;
     double overall_parr=duration<double,nano>(end-start).count();
    cout<<"Prallel one "<< overall_parr<<"\n";
    cout<<"Speed up Factor : "<< overall_seq/overall_parr<<"\n";
    
    vector<int> arr3=arr;
     //  start=omp_get_wtime();
      start=high_resolution_clock::now();
    merge_sort(arr3,0,arr3.size()-1);
    //end=omp_get_wtime();
     end=high_resolution_clock::now();
   // double overall_seq_m=end-start;
    double overall_seq_m=duration<double,nano>(end-start).count();
    cout<<"Sequential one "<< overall_seq_m<<"\n";


    vector<int> arr4=arr;
    // start=omp_get_wtime();
     start=high_resolution_clock::now();
   merge_sort_p(arr4,0,arr4.size()-1);
    //end=omp_get_wtime();
     end=high_resolution_clock::now();
   // double overall_parr_m=end-start;
  double overall_parr_m=duration<double,nano>(end-start).count();
    cout<<"Parallel one "<< overall_parr_m<<"\n";
    cout<<"Speed up Factor : "<< overall_seq_m/overall_parr_m<<"\n";  
    
}