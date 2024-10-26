/**
 * @brief		This file contains the types definitions.
 * @file
 */

/**
 * @defgroup	doxy_cothread_types		types
 */

#ifndef __COTHREAD_TYPES_H__
#define __COTHREAD_TYPES_H__

/**
 * @brief		The error type.
 * @ingroup		doxy_cothread_types
 */
enum _cothread_err_t
{
	cothread_err_ok,		///< @brief	No error.
	cothread_err_notsup,	///< @brief	Operation not supported.
};

/**
 * @brief		Says whether the specified error means "success" or not.
 * @ingroup		doxy_cothread_types
 */
#define COTHREAD_ERR_ISOK(_err)	(cothread_err_ok == (_err))

/**
 * @brief		Says whether the specified error means "failure" or not.
 * @ingroup		doxy_cothread_types
 */
#define COTHREAD_ERR_ISNOK(_err)	(!COTHREAD_ERR_ISOK(_err))

/// @ingroup	doxy_cothread_types
/// @{
typedef enum _cothread_err_t		cothread_err_t;	///< @brief		The error type.
/// @}

#endif /* __COTHREAD_TYPES_H__ */
