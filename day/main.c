#include <stdio.h>
#include <stdbool.h>

// Function prototypes
int is_leap(int year);
int get_days_in_month(int month, int year);
bool is_valid_date(int day, int month, int year);
int calculate_day_of_week(int day, int month, int year);
void print_day_name(int day_index);

/**
 * @brief Checks if a given year is a leap year.
 * * A year is a leap year if it is divisible by 4, unless it is divisible by 100
 * but not by 400.
 * * @param year The year to check.
 * @return 1 if it is a leap year, 0 otherwise.
 */
int is_leap(int year) {
    if (year % 400 == 0) {
        return 1;
    }
    if (year % 100 == 0) {
        return 0;
    }
    if (year % 4 == 0) {
        return 1;
    }
    return 0;
}

/**
 * @brief Gets the number of days in a specific month of a specific year.
 * * @param month The month (1-12).
 * @param year The year.
 * @return The number of days, or -1 if the month is invalid.
 */
int get_days_in_month(int month, int year) {
    if (month < 1 || month > 12) {
        return -1; // Invalid month
    }

    // 30 days: April (4), June (6), September (9), November (11)
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }

    // 31 days: January (1), March (3), May (5), July (7), August (8), October (10), December (12)
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        return 31;
    }

    // February (2)
    if (month == 2) {
        return is_leap(year) ? 29 : 28;
    }

    return -1; // Should not happen
}

/**
 * @brief Validates if the given day, month, and year form a correct date.
 * * @param day The day of the month.
 * @param month The month.
 * @param year The year.
 * @return true if the date is valid, false otherwise.
 */
bool is_valid_date(int day, int month, int year) {
    // Check reasonable year range (e.g., common era)
    if (year < 1700 || year > 2500) {
        printf("Error: Year must be between 1700 and 2500 for this calculation.\n");
        return false;
    }

    if (month < 1 || month > 12) {
        printf("Error: Month must be between 1 and 12.\n");
        return false;
    }

    int max_day = get_days_in_month(month, year);
    if (day < 1 || day > max_day) {
        printf("Error: Day must be between 1 and %d for %d/%d.\n", max_day, month, year);
        return false;
    }

    return true;
}

/**
 * @brief Calculates the day of the week using a variation of Zeller's Congruence.
 * * The calculation returns an index (0=Sunday, 1=Monday, ..., 6=Saturday).
 * * @param day Day of the month.
 * @param month Month (1-12).
 * @param year Year (e.g., 2025).
 * @return An integer (0-6) representing the day of the week.
 */
int calculate_day_of_week(int day, int month, int year) {
    // If month is January (1) or February (2), treat it as month 13 or 14 of the previous year.
    if (month == 1) {
        month = 13;
        year--;
    } else if (month == 2) {
        month = 14;
        year--;
    }

    // Zeller's Congruence adaptation:
    // (k + floor((13*m - 1) / 5) + D + floor(D/4) + floor(C/4) - 2*C) mod 7
    // Where:
    // k = day (d)
    // m = adjusted month (3-14)
    // D = year of century (y % 100)
    // C = century (y / 100)

    int D = year % 100; // Year of the century
    int C = year / 100; // Century

    // The formula results in 0=Sunday, 1=Monday, ..., 6=Saturday.
    int day_index = (day +
                     (13 * (month + 1)) / 5 +
                     D +
                     D / 4 +
                     C / 4 +
                     5 * C) % 7;

    // Zeller's result: 0=Saturday, 1=Sunday, ..., 6=Friday.
    // The given formula is a slight modification where:
    // day_index = (day + ((13*month-1)/5) + D + (D/4) + (C/4) - 2*C) % 7;
    // And adjusts the modulus 7 result to be 0=Sunday, 1=Monday...

    // Using this common simplified form:
    // day_index = (day + (13*(month+1))/5 + D + D/4 + C/4 - 2*C) % 7;
    // The 5*C adjustment makes it congruent to (Saturday=0... Friday=6)

    // Let's use the standard Zeller's Congruence (Saturday=0, Sunday=1, ..., Friday=6)
    // k is day, m is month, D is year of century, C is century
    int h = (day +
             ((13 * (month + 1)) / 5) +
             D +
             (D / 4) +
             (C / 4) +
             5 * C) % 7;

    // Since the standard Zeller's returns 0=Saturday, 1=Sunday, 2=Monday, ...
    // we return the index directly.
    return h;
}

/**
 * @brief Prints the name of the day corresponding to the index (0=Saturday, 1=Sunday, ...).
 * * @param day_index The calculated day index (0-6).
 */
void print_day_name(int day_index) {
    // 0=Saturday, 1=Sunday, 2=Monday, 3=Tuesday, 4=Wednesday, 5=Thursday, 6=Friday
    const char *day_names[] = {
        "Saturday", "Sunday", "Monday", "Tuesday",
        "Wednesday", "Thursday", "Friday"
    };

    if (day_index >= 0 && day_index <= 6) {
        printf("The day of the week was: %s\n", day_names[day_index]);
    } else {
        printf("Error: Invalid day index calculated.\n");
    }
}

/**
 * @brief Main function to handle user input and process the date.
 */
int main() {
    int day, month, year;

    printf("--- Day of the Week Calculator ---\n");
    printf("Enter a date in the format DD/MM/YYYY (e.g., 15/10/2025): ");

    // Read input from the user in dd/mm/yyyy format
    if (scanf("%d/%d/%d", &day, &month, &year) != 3) {
        printf("Input Error: Please ensure the format is exactly DD/MM/YYYY with numbers.\n");
        return 1;
    }

    // Validate the date before calculation
    if (!is_valid_date(day, month, year)) {
        printf("Exiting program due to invalid date.\n");
        return 1;
    }

    // Calculate the day index
    int day_index = calculate_day_of_week(day, month, year);

    // Output the result
    printf("\n--- Result ---\n");
    printf("Date entered: %d/%d/%d\n", day, month, year);
    print_day_name(day_index);
    printf("----------------\n");

    return 0;
}
