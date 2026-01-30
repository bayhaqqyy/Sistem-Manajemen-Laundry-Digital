#include <cstdio>
#include <cstring>
#include <cctype>
#include <ctime>
#include <clocale>

#define MAX_ORDERS 100
#define MAX_NAME 51
#define MAX_TYPE 20
#define MAX_SERVICE 20
#define STATUS_COUNT 5

const char *status_labels[STATUS_COUNT] = {
    "Menunggu", "Dicuci", "Dikeringkan", "Disetrika", "Selesai"
};

typedef struct {
    int id;
    char customer[MAX_NAME];
    char clothing[MAX_TYPE];
    float weight;
    char service[MAX_SERVICE];
    int priority;
    double price;
    int status_index;
    bool canceled;
} Order;

Order orders[MAX_ORDERS];
int order_count = 0;

int status_timeline[MAX_ORDERS][STATUS_COUNT];

void print_header(void) {
    char date_buf[64] = "";
    std::time_t now = std::time(nullptr);
    std::tm *local = std::localtime(&now);
    if (local != nullptr) {
        const char *days[] = {
            "Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"
        };
        const char *months[] = {
            "Januari", "Februari", "Maret", "April", "Mei", "Juni",
            "Juli", "Agustus", "September", "Oktober", "November", "Desember"
        };
        int wday = local->tm_wday;
        int mon = local->tm_mon;
        if (wday >= 0 && wday < 7 && mon >= 0 && mon < 12) {
            std::snprintf(
                date_buf,
                sizeof(date_buf),
                "%s, %02d %s %d",
                days[wday],
                local->tm_mday,
                months[mon],
                1900 + local->tm_year
            );
        }
    }
    printf("=== LAUNDRYEXPRESS PRO MANAGEMENT SYSTEM ===\n");
    if (date_buf[0] != '\0') {
        printf("Hari: %s\n", date_buf);
    }
    printf("Total Order Hari Ini: %d\n", order_count);
    int completed = 0;
    int in_process = 0;
    for (int i = 0; i < order_count; i++) {
        if (orders[i].status_index >= STATUS_COUNT - 1) {
            completed++;
        } else {
            in_process++;
        }
    }
    printf("Order Selesai: %d\n", completed);
    printf("Order Dalam Proses: %d\n\n", in_process);
}

bool is_valid_clothing(const char *input) {
    const char *options[] = {"Baju", "Celana", "Jaket", "Selimut", "Lainnya"};
    for (int i = 0; i < 5; i++) {
        if (strcmp(input, options[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool is_valid_service(const char *input) {
    const char *options[] = {"Express", "Fast", "Normal"};
    for (int i = 0; i < 3; i++) {
        if (strcmp(input, options[i]) == 0) {
            return true;
        }
    }
    return false;
}

double clothing_multiplier(const char *type) {
    if (strcmp(type, "Baju") == 0) {
        return 1.0;
    }
    if (strcmp(type, "Celana") == 0) {
        return 1.2;
    }
    if (strcmp(type, "Jaket") == 0) {
        return 1.5;
    }
    if (strcmp(type, "Selimut") == 0) {
        return 2.0;
    }
    return 1.3;
}

double service_multiplier(const char *service) {
    if (strcmp(service, "Express") == 0) {
        return 2.0;
    }
    if (strcmp(service, "Fast") == 0) {
        return 1.5;
    }
    return 1.0;
}

int count_orders_by_customer(const char *customer) {
    int count = 0;
    for (int i = 0; i < order_count; i++) {
        if (strcmp(orders[i].customer, customer) == 0) {
            count++;
        }
    }
    return count;
}

double calculate_price(const char *type, const char *service, float weight, const char *customer) {
    double base = 5000.0 * weight;
    double total = base * clothing_multiplier(type) * service_multiplier(service);

    if (weight > 10.0f) {
        total *= 0.9;
    }

    int customer_orders = 0;
    for (int i = 0; i < order_count; i++) {
        if (strcmp(orders[i].customer, customer) == 0) {
            customer_orders++;
        }
    }

    if (customer_orders >= 2) {
        total *= 0.85;
    }

    return total;
}

void input_order(void) {
    if (order_count >= MAX_ORDERS) {
        printf("Kapasitas order penuh.\n");
        return;
    }

    Order new_order = {0};
    new_order.id = order_count + 1;

    printf("Nama pelanggan (max 50): ");
    fgets(new_order.customer, sizeof(new_order.customer), stdin);
    new_order.customer[strcspn(new_order.customer, "\n")] = '\0';
    if (strlen(new_order.customer) == 0 || strlen(new_order.customer) > 50) {
        printf("Nama tidak valid.\n");
        return;
    }

    printf("Jenis pakaian (Baju/Celana/Jaket/Selimut/Lainnya): ");
    fgets(new_order.clothing, sizeof(new_order.clothing), stdin);
    new_order.clothing[strcspn(new_order.clothing, "\n")] = '\0';
    if (!is_valid_clothing(new_order.clothing)) {
        printf("Jenis pakaian tidak valid.\n");
        return;
    }

    printf("Berat (0.5 - 20 kg): ");
    if (scanf("%f", &new_order.weight) != 1 || new_order.weight < 0.5f || new_order.weight > 20.0f) {
        printf("Berat tidak valid.\n");
        while (getchar() != '\n') {
        }
        return;
    }
    while (getchar() != '\n') {
    }

    printf("Jenis layanan (Express/Fast/Normal): ");
    fgets(new_order.service, sizeof(new_order.service), stdin);
    new_order.service[strcspn(new_order.service, "\n")] = '\0';
    if (!is_valid_service(new_order.service)) {
        printf("Layanan tidak valid.\n");
        return;
    }

    printf("Prioritas (1-5): ");
    if (scanf("%d", &new_order.priority) != 1 || new_order.priority < 1 || new_order.priority > 5) {
        printf("Prioritas tidak valid.\n");
        while (getchar() != '\n') {
        }
        return;
    }
    while (getchar() != '\n') {
    }

    new_order.price = calculate_price(new_order.clothing, new_order.service, new_order.weight, new_order.customer);
    new_order.status_index = 0;
    new_order.canceled = false;

    orders[order_count] = new_order;
    for (int i = 0; i < STATUS_COUNT; i++) {
        status_timeline[order_count][i] = 0;
    }
    status_timeline[order_count][0] = 1;
    order_count++;

    printf("Order berhasil ditambahkan. Total biaya: Rp %.0f\n", new_order.price);
}

void update_status(void) {
    for (int i = 0; i < order_count; i++) {
        if (orders[i].status_index >= STATUS_COUNT - 1) {
            continue;
        }
        if (orders[i].priority == 1) {
            printf("Order urgent ditemukan (ID %d). Prioritas tinggi diproses dulu.\n", orders[i].id);
            break;
        }
        orders[i].status_index++;
        status_timeline[i][orders[i].status_index] = 1;
    }
    printf("Status order diperbarui.\n");
}

void display_dashboard(void) {
    printf("\nDASHBOARD STATUS ORDER\n");
    printf("ID | Pelanggan | Status\n");
    for (int i = 0; i < order_count; i++) {
        printf("%d | %s | %s\n", orders[i].id, orders[i].customer, status_labels[orders[i].status_index]);
    }

    printf("\nTimeline (1=aktif):\n");
    printf("ID ");
    for (int s = 0; s < STATUS_COUNT; s++) {
        printf("| %s ", status_labels[s]);
    }
    printf("\n");

    for (int i = 0; i < order_count; i++) {
        printf("%d ", orders[i].id);
        for (int s = 0; s < STATUS_COUNT; s++) {
            printf("| %d ", status_timeline[i][s]);
        }
        printf("\n");
    }
}

void sort_by_priority(int *order_ids, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            int id_a = order_ids[j] - 1;
            int id_b = order_ids[j + 1] - 1;
            if (orders[id_a].priority > orders[id_b].priority) {
                int temp = order_ids[j];
                order_ids[j] = order_ids[j + 1];
                order_ids[j + 1] = temp;
            }
        }
    }
}

float calculate_efficiency(float *weights, int *durations, int size) {
    float total = 0.0f;
    for (int i = 0; i < size; i++) {
        if (durations[i] == 0) {
            continue;
        }
        total += weights[i] / durations[i];
    }
    return size > 0 ? total / size : 0.0f;
}

void suggest_machine(int type, float weight, int *machine_id) {
    if (weight > 15.0f) {
        *machine_id = 3;
    } else if (type == 3) {
        *machine_id = 2;
    } else {
        *machine_id = 1;
    }
}

void optimize_washing(void) {
    int order_ids[MAX_ORDERS];
    float weights[MAX_ORDERS];
    int durations[MAX_ORDERS];

    for (int i = 0; i < order_count; i++) {
        order_ids[i] = orders[i].id;
        weights[i] = orders[i].weight;
        if (strcmp(orders[i].service, "Express") == 0) {
            durations[i] = 3;
        } else if (strcmp(orders[i].service, "Fast") == 0) {
            durations[i] = 6;
        } else {
            durations[i] = 24;
        }
    }

    sort_by_priority(order_ids, order_count);
    float efficiency = calculate_efficiency(weights, durations, order_count);

    printf("Urutan berdasarkan prioritas: ");
    for (int i = 0; i < order_count; i++) {
        printf("%d ", order_ids[i]);
    }
    printf("\nEfisiensi mesin rata-rata: %.2f kg/jam\n", efficiency);

    for (int i = 0; i < order_count; i++) {
        int machine = 0;
        suggest_machine(i % 4, orders[i].weight, &machine);
        printf("Order %d disarankan ke mesin %d\n", orders[i].id, machine);
    }
}

void lower_copy(char *dest, const char *src) {
    for (int i = 0; src[i] != '\0'; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
        dest[i + 1] = '\0';
    }
}

void search_orders(void) {
    int choice = 0;
    printf("\nSISTEM PENCARIAN\n");
    printf("1. Cari nama pelanggan\n2. Cari status\n3. Cari rentang ID\nPilih: ");
    scanf("%d", &choice);
    while (getchar() != '\n') {
    }

    if (choice == 1) {
        char query[MAX_NAME];
        printf("Masukkan nama: ");
        fgets(query, sizeof(query), stdin);
        query[strcspn(query, "\n")] = '\0';

        char query_lower[MAX_NAME] = {0};
        lower_copy(query_lower, query);

        printf("Hasil: ");
        for (int i = 0; i < order_count; i++) {
            char customer_lower[MAX_NAME] = {0};
            lower_copy(customer_lower, orders[i].customer);
            if (strstr(customer_lower, query_lower)) {
                printf("Order ID: %d ", orders[i].id);
            }
        }
        printf("\n");
    } else if (choice == 2) {
        char status[MAX_SERVICE];
        printf("Status (Menunggu/Dicuci/Dikeringkan/Disetrika/Selesai): ");
        fgets(status, sizeof(status), stdin);
        status[strcspn(status, "\n")] = '\0';

        printf("Hasil: ");
        for (int i = 0; i < order_count; i++) {
            if (strcmp(status_labels[orders[i].status_index], status) == 0) {
                printf("Order ID: %d ", orders[i].id);
            }
        }
        printf("\n");
    } else if (choice == 3) {
        int start = 0;
        int end = 0;
        printf("Rentang ID (start end): ");
        scanf("%d %d", &start, &end);
        while (getchar() != '\n') {
        }
        printf("Hasil: ");
        for (int i = 0; i < order_count; i++) {
            if (orders[i].id >= start && orders[i].id <= end) {
                printf("Order ID: %d ", orders[i].id);
            }
        }
        printf("\n");
    }
}

void generate_report(void) {
    double total_revenue = 0.0;
    float total_weight = 0.0f;
    int service_count[3] = {0};
    int max_orders = 0;
    char top_customer[MAX_NAME] = "";

    if (order_count == 0) {
        printf("Data tidak valid untuk laporan.\n");
        return;
    }

    for (int i = 0; i < order_count; i++) {
        if (orders[i].canceled) {
            continue;
        }
        total_revenue += orders[i].price;
        total_weight += orders[i].weight;

        if (strcmp(orders[i].service, "Express") == 0) {
            service_count[0]++;
        } else if (strcmp(orders[i].service, "Fast") == 0) {
            service_count[1]++;
        } else {
            service_count[2]++;
        }
    }

    for (int i = 0; i < order_count && i < 5; i++) {
        int count = count_orders_by_customer(orders[i].customer);
        if (count > max_orders) {
            max_orders = count;
            strncpy(top_customer, orders[i].customer, MAX_NAME - 1);
            top_customer[MAX_NAME - 1] = '\0';
        }
        if (i == 4) {
            break;
        }
    }

    int popular_index = 0;
    for (int i = 1; i < 3; i++) {
        if (service_count[i] > service_count[popular_index]) {
            popular_index = i;
        }
    }

    const char *popular_service = popular_index == 0 ? "Express" : (popular_index == 1 ? "Fast" : "Normal");

    printf("\nLAPORAN HARIAN\n");
    printf("Total pendapatan: Rp %.0f\n", total_revenue);
    printf("Rata-rata berat: %.2f kg\n", total_weight / order_count);
    printf("Layanan populer: %s\n", popular_service);
    printf("Pelanggan terbanyak: %s (%d order)\n", top_customer, max_orders);
    printf("Estimasi kapasitas besok: %.2f kg\n", total_weight * 1.1f);
}

int recursive_predict(int hours, int stages) {
    if (stages <= 0) {
        return 0;
    }
    return hours + recursive_predict(hours, stages - 1);
}

void estimate_time_and_cost(void) {
    if (order_count == 0) {
        printf("Belum ada order.\n");
        return;
    }
    int id = 0;
    printf("Masukkan ID order: ");
    scanf("%d", &id);
    while (getchar() != '\n') {
    }
    if (id < 1 || id > order_count) {
        printf("ID tidak ditemukan.\n");
        return;
    }
    Order *order = &orders[id - 1];
    int hours = strcmp(order->service, "Express") == 0 ? 3 : (strcmp(order->service, "Fast") == 0 ? 6 : 24);
    int remaining = STATUS_COUNT - 1 - order->status_index;
    int estimate = recursive_predict(hours, remaining);
    printf("Estimasi sisa waktu: %d jam, Biaya: Rp %.0f\n", estimate, order->price);
}

void menu_loop(void) {
    int choice = 0;
    do {
        print_header();
        printf("MENU UTAMA:\n");
        printf("1. Tambah Order Baru\n");
        printf("2. Proses Order (Update Status)\n");
        printf("3. Cari & Lihat Detail Order\n");
        printf("4. Hitung Estimasi Waktu & Biaya\n");
        printf("5. Generate Laporan Harian\n");
        printf("6. Optimasi Urutan Pencucian\n");
        printf("7. Dashboard Status Order\n");
        printf("8. Reset Data Harian\n");
        printf("9. Keluar\n");
        printf("Pilih menu: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n') {
            }
            continue;
        }
        while (getchar() != '\n') {
        }

        switch (choice) {
            case 1:
                input_order();
                break;
            case 2:
                update_status();
                break;
            case 3:
                search_orders();
                break;
            case 4:
                estimate_time_and_cost();
                break;
            case 5:
                generate_report();
                break;
            case 6:
                optimize_washing();
                break;
            case 7:
                display_dashboard();
                break;
            case 8:
                order_count = 0;
                printf("Data harian direset.\n");
                break;
            case 9:
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
        printf("\n");
    } while (choice != 9);
}

int main(void) {
    menu_loop();
    return 0;
}
