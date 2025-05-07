#include<iostream>
#include<vector> 
#include<chrono> 
#include<omp.h>
using namespace std;
using namespace std::chrono;
void seq(vector<double> &x,vector<double> &y,double &beta0,double &beta1,int n){
    double sum_x=0.0;
    double sum_y=0.0;
    double sum_xx=0.0;
    double sum_xy=0.0;
    for(int i=0;i<x.size();i++){
        sum_x+=x[i];
        sum_y+=y[i];
        sum_xx+=x[i]*x[i];
        sum_xy+=x[i]*y[i];
    }
    beta1=(n*sum_xy-sum_x*sum_y)/(n*sum_xx-sum_x*sum_x);
    beta0=(sum_y-beta1*sum_x)/n;
    cout<<"Line equation is given by : "<<"\n";
    cout<<" y = "<<beta1<<" x + "<< beta0;
}
void parr(vector<double> &x,vector<double> &y,double &beta0,double &beta1,int n){
    double sum_x=0.0;
    double sum_y=0.0;
    double sum_xx=0.0;
    double sum_xy=0.0;
    #pragma omp parallel for reduction(+:sum_x,sum_y,sum_xx,sum_xy)
    for(int i=0;i<x.size();i++){
        sum_x+=x[i];
        sum_y+=y[i];
        sum_xx+=x[i]*x[i];
        sum_xy+=x[i]*y[i];
    }
    beta1=(n*sum_xy-sum_x*sum_y)/(n*sum_xx-sum_x*sum_x);
    beta0=(sum_y-beta1*sum_x)/n;
    cout<<"Line equation is given by : "<<"\n";
    cout<<" y = "<<beta1<<" x + "<< beta0;
}
int main(){
    int n=5;
    vector<double> x(n);
    vector<double> y(n);
    x={1.0,2.0,3.0,4.0,5.0};
    y={2.0,4.0,5.0,4.0,5.0};
      
    double beta0_seq = 0.0, beta1_seq = 0.0;
    double beta0_par = 0.0, beta1_par = 0.0;
    auto start=high_resolution_clock::now();
   // omp_set_num_threads(omp_get_max_threads());
    //auto start=high_resolution_clock::now();
    //double start=omp_get_wtime();
    seq(x,y,beta0_seq,beta1_seq,n);  // ✅ Correct
    auto end=high_resolution_clock::now();
    double time_1=duration<double,nano>(end-start).count();
    //double end=omp_get_wtime();
   // double seq_t=(end-start);
    //cout<<"   Time req is : "<<(end-start)<<"\n";
    //start= omp_get_wtime();
    start=high_resolution_clock::now();
    parr(x,y,beta0_par,beta1_par,n);
     end=high_resolution_clock::now();
    double time_2=duration<double,nano>(end-start).count();
    // end=omp_get_wtime();
    // double parr_t=end-start;
   // cout<<"   Time req is : "<<(end-start)<<"\n";
    //cout<<"Speed Up factor is : "<<(seq_t/parr_t);
    cout<<"\n"<<"Speed Up factor is : "<<(time_1/time_2);


}
