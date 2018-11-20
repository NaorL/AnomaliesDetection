/**
 * Detect timing anomalies in the provided file.
 * Assume that the provided file will not contain any anomalies other
 * than timing anomalies.
 *
 * @param [in]  file_path	  Full path to the log file with messages.
 * 							  Assume that if the file exists and is accessible,
 * 							  it is in the proper format.
 * @param [out] anomalous_ids An array of size 1000, initialized to 0 by the caller.
 *							  On return, the array should contain the ids of anomalous messages,
 *							  in an increasing order.
 * 							  If there were less than 1000 anomalies (or none), fill only the relevant part of the
 * 							  array, leaving all the rest intact.
 * 							  Each line should only be reported once.
 * 							  Assume that the array exists and has the proper size.
 *
 * @return 0 - in case of SUCCESS, -1 if some internal error occurred.
 */
int detect_timing_anomalies(const char* file_path, unsigned int *anomalies_ids);

/**
 * Detect behavioral anomalies in the provided file.
 * Assume that the provided file will not contain any anomalies other
 * than behavioral anomalies.
 *
 * @param [in]  file_path	  Full path to the log file with messages.
 * 							  Assume that if the file exists and is accessible,
 * 							  it is in the proper format.
 * @param [out] anomalous_ids An array of size 1000, initialized to 0 by the caller.
 *							  On return, the array should contain the ids of anomalous messages,
 *							  in an increasing order.
 * 							  If there were less than 1000 anomalies (or none), fill only the relevant part of the
 * 							  array, leaving all the rest intact.
 * 							  Each line should only be reported once.
 * 							  Assume that the array exists and has the proper size.
 *
 * * @return 0 - in case of SUCCESS, -1 if some internal error occurred.
 */
int detect_behavioral_anomalies(const char* file_path, unsigned int *anomalies_ids);


/**
 * Detect correlation anomalies in the provided file, as defined in the exercise.
 * Assume that the provided file will not contain any anomalies other
 * than correlation anomalies.
 *
 * @param [in]  file_path	  Full path to the log file with messages.
 * 							  Assume that if the file exists and is accessible,
 * 							  it is in the proper format.
 * @param [out] anomalous_ids An array of size 1000, initialized to 0 by the caller.
 *							  On return, the array should contain the ids of anomalous messages,
 *							  in an increasing order.
 * 							  If there were less than 1000 anomalies (or none), fill only the relevant part of the
 * 							  array, leaving all the rest intact.
 * 							  Each line should only be reported once.
 * 							  You may assume that the array exists and has the proper size.
 *
 * * @return 0 - in case of SUCCESS, -1 if some internal error occurred.
 */
int detect_correlation_anomalies(const char* file_path, unsigned int *anomalies_ids);