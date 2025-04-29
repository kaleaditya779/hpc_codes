#include<iostream>
#include<vector>
#include<omp.h>


using namespace std;

// Finding minimum value in array using parallel reduction:
int min_reduction(vector<int> &arr){
    int min_value = arr[0];
    #pragma omp parallel for reduction(min:min_value)
    for (int i=0; i<arr.size(); i++) {
        if (arr[i]<min_value){
            min_value=arr[i];
        }
    }
    return min_value;
}

// finding max value from array using parallel reduction:
int max_reduction(vector<int> &arr){
    int max_value=arr[0];
    #pragma omp parallel for reduction(max:max_value)
    for (int i=0; i<arr.size(); i++){
        if (arr[i]>max_value){
            max_value=arr[i];
        }
    }
    return max_value;
}

// finding sum of all elements in array:
int sum_reduction(vector<int> &arr){
    int sum=0;
    #pragma omp parallel for reduction(+:sum)
    for (int i=0; i<arr.size(); i++){
        sum += arr[i];
    }
    return sum;
}

// finding avg of array using parallel reduction:
float average_reduction(vector<int> &arr){
    int sum = sum_reduction(arr);
    float avg = sum/arr.size();
    return avg;
}

int main(){
    cout<<endl<<"Enter the size of array: ";
    int si;
    cin>>si;
    
    vector<int> ar(si, 0);

    cout<<endl<<"Enter each element of array one by one: ";
    for (int i=0; i<si; i++){
        cin>>ar[i];
    }
    int choice;
    do {
        cout<<endl<<"1. Min reduction\n2. Max reduction\n3. Sum reduction\n4. Average reduction\n5. Exit";
        cout<<endl<<"Your choice: ";
        cin>>choice;

        if (choice==1){
            int mini = min_reduction(ar);
            cout<<endl<<"The min is: "<<mini;
        } else if (choice==2){
            int maxi = max_reduction(ar);
            cout<<endl<<"The max is: "<<maxi;
        } else if (choice==3){
            int sumi = sum_reduction(ar);
            cout<<endl<<"The sum is: "<<sumi;
        } else if (choice==4){
            int avgi = average_reduction(ar);
            cout<<endl<<"The average is: "<<avgi;
        } else if (choice != 5) {
            cout<<endl<<"Wrong input!! Try again..";
        }
        

    } while (choice!=5);


    return 0;

}