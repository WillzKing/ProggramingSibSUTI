void shaker_sort(int arr[], int n, int *swaps, int *comparisons) {
    *swaps = 0;
    *comparisons = 0;
    int left = 0;
    int right = n - 1;
    int swapped = 1; // Флаг для отслеживания пересылок

    while (left <= right && swapped) {
        swapped = 0;

        // Проход слева направо
        for (int i = left; i < right; i++) {
            (*comparisons)++;
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                (*swaps) += 3; // Каждая перестановка требует 3 операции
                swapped = 1;
            }
        }
        right--;

        // Проход справа налево
        for (int i = right; i > left; i--) {
            (*comparisons)++;
            if (arr[i - 1] > arr[i]) {
                int temp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = temp;
                (*swaps) += 3; // Каждая перестановка требует 3 операции
                swapped = 1;
            }
        }
        left++;
    }
}
int main(){

    int a[3] = {4, 0};
    shaker_sort(a[3]);
}
//AR