#include "StoredVarible.hpp"

// Function to read the stored boolean from the Registry
bool readStoredBoolean() {
    HKEY hKey;
    DWORD storedBoolean = 0;

    // Open (or create if not exists) the registry key
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\MyApp", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD dataSize = sizeof(DWORD);
        DWORD dataType;
        if (RegQueryValueEx(hKey, "StoredBoolean", NULL, &dataType, (LPBYTE)&storedBoolean, &dataSize) != ERROR_SUCCESS) {
            storedBoolean = 0;  // Default value if the value doesn't exist
        }
        RegCloseKey(hKey);
    }

    return storedBoolean != 0;
}

// Function to update and store the boolean in the Registry
void updateStoredBoolean(bool newValue) {
    HKEY hKey;
    DWORD storedBoolean = newValue ? 1 : 0;

    // Create or open the registry key
    if (RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\MyApp", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, "StoredBoolean", 0, REG_DWORD, (const BYTE*)&storedBoolean, sizeof(storedBoolean));
        RegCloseKey(hKey);
    }
}


// Function to read the stored integer from the Registry
int readStoredInteger() {
    HKEY hKey;
    int storedInteger = 0;

    // Open (or create if not exists) the registry key
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\MyApp", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD dataSize = sizeof(int);
        DWORD dataType;
        if (RegQueryValueEx(hKey, "StoredInteger", NULL, &dataType, (LPBYTE)&storedInteger, &dataSize) != ERROR_SUCCESS) {
            storedInteger = 0;  // Default value if the value doesn't exist
        }
        RegCloseKey(hKey);
    }

    return storedInteger;
}

// Function to update and store the integer in the Registry
void updateStoredInteger(int newValue) {
    HKEY hKey;

    // Create or open the registry key
    if (RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\MyApp", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, "StoredInteger", 0, REG_DWORD, (const BYTE*)&newValue, sizeof(newValue));
        RegCloseKey(hKey);
    }
}

// Function to read the stored string from the Registry
std::string readStoredString() {
    HKEY hKey;
    std::string storedString;

    // Open (or create if not exists) the registry key
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\MyApp", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD dataType;
        DWORD dataSize = 0;

        // Query the size of the string value
        if (RegQueryValueEx(hKey, "StoredString", NULL, &dataType, NULL, &dataSize) == ERROR_SUCCESS && dataType == REG_SZ) {
            // Allocate buffer for the string
            std::vector<char> buffer(dataSize);

            // Read the string value
            if (RegQueryValueEx(hKey, "StoredString", NULL, NULL, reinterpret_cast<LPBYTE>(buffer.data()), &dataSize) == ERROR_SUCCESS) {
                storedString.assign(buffer.data());
            }
        }

        RegCloseKey(hKey);
    }

    return storedString;
}

// Function to update and store the string in the Registry
void updateStoredString(const std::string& newValue) {
    HKEY hKey;

    // Create or open the registry key
    if (RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\MyApp", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        // Convert string to LPCSTR (const char*) for Registry storage
        const char* strValue = newValue.c_str();

        // Set the string value in the Registry
        RegSetValueEx(hKey, "StoredString", 0, REG_SZ, reinterpret_cast<const BYTE*>(strValue), static_cast<DWORD>(strlen(strValue) + 1));

        RegCloseKey(hKey);
    }
}