#ifndef JSON_MODULE_H
#define JSON_MODULE_H

/**
 * @brief This function extracts a string value associated with
 * a given key from a JSON-formatted string and stores it in the provided buffer.
 *
 * @param json_str (const char *): The JSON-formatted string input.
 * @param key (const char *): The key whose corresponding string value needs to be extracted.
 * @param output_string (char *): A buffer where the extracted string value will be stored.
 * @param output_size (size_t): The size of the output_string buffer to prevent buffer overflows.
 *
 * @return bool
 * - Returns true if the value is successfully extracted and valid.
 * - Returns false if the JSON is invalid, the key is missing, or the value is not a string.
 */
bool JSON_ExtractString(const char *json_str, const char *key, char *string, size_t max_len);



/**
 * @brief This function extracts an integer value associated with
 * a given key from a JSON-formatted string and stores it in the provided variable.
 *
 * @param json_str (const char *): The JSON string input.
 * @param key (const char *): The key whose corresponding integer value needs to be extracted.
 * @param output_value (int32_t *): A pointer to an integer where the extracted value will be stored.
 *
 * @return bool
 * - Returns true if the value is successfully extracted and valid.
 * - Returns false if the JSON is invalid, the key is missing, or the value is not an integer.
 */
bool JSON_ExtractInt32(const char *json_str, const char *key, int32_t *value);

#endif // JSON_MODULE_H
