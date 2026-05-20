#ifndef STORAGE_ERRORS_HPP
#define STORAGE_ERRORS_HPP

#include <stdexcept>
#include <string>

/*
*
*/
enum class StorageErrorCode {
    FileOpenFailed,
    FileReadFailed,
    FileWriteFailed,
    TempFileOpenFailed,
    FileRemoveFailed,
    FileRenameFailed,
    FileSeekFailed,
    Unknown
};

/*
* Reusable error message fragments.
*/
inline constexpr const char* OPEN_DB_FILE =
    "Could not open database file: ";

inline constexpr const char* OPEN_TEMP_FILE =
    "Could not open temporary database file: ";

inline constexpr const char* WRITE_DB_FILE =
    "Could not write to database file: ";

inline constexpr const char* READ_DB_FILE =
    "Error while reading from database file: ";

inline constexpr const char* UPDATE_DB_FILE =
    "Error while updating database file: ";

inline constexpr const char* REMOVE_DB_FILE =
    "Could not remove database file: ";

inline constexpr const char* RENAME_DB_FILE =
    "Could not rename temporary database file: ";

inline constexpr const char* SEEK_DB_FILE =
    "Could not seek inside database file: ";

    
inline std::string errorCodeToString(StorageErrorCode code) {
    switch (code) {
        case StorageErrorCode::FileOpenFailed:
            return "File open failed";

        case StorageErrorCode::FileReadFailed:
            return "File read failed";

        case StorageErrorCode::FileWriteFailed:
            return "File write failed";

        case StorageErrorCode::TempFileOpenFailed:
            return "Temporary file open failed";

        case StorageErrorCode::FileRemoveFailed:
            return "File remove failed";

        case StorageErrorCode::FileRenameFailed:
            return "File rename failed";

        case StorageErrorCode::FileSeekFailed:
            return "File seek failed";

        case StorageErrorCode::Unknown:
        default:
            return "Unknown storage error";
    }
}

class StorageException : public std::runtime_error {
private:
    StorageErrorCode code;

public:
    StorageException(StorageErrorCode code, const std::string& details)
        : std::runtime_error(errorCodeToString(code) + ": " + details),
          code(code) {
    }

    StorageErrorCode getCode() const {
        return code;
    }
};

#endif