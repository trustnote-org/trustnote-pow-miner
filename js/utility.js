'use strict';


/**
 *	@namespace	Utilities
 */
class Utilities
{
	static isNumeric( oObj )
	{
		return ( ! isNaN( oObj ) && "[object Number]" === Object.prototype.toString.call( oObj ) );
	}

	static isString( oObj )
	{
		return ( "[object String]" === Object.prototype.toString.call( oObj ) );
	}

	static isBool( oObj )
	{
		return ( "[object Boolean]" === Object.prototype.toString.call( oObj ) );
	}

	static isObject( oObj )
	{
		return ( "[object Object]" === Object.prototype.toString.call( oObj ) ||
			"[object Blob]" === Object.prototype.toString.call( oObj ) );
	}

	static isFunction( oObj )
	{
		return ( "[object Function]" === Object.prototype.toString.call( oObj ) );
	}

	/**
	 *	check if an object is a valid Date object
	 *
	 *	@param	oObj	{object}
	 *	@return {boolean}
	 */
	static isValidDateObject( oObj )
	{
		//	d.valueOf() could also work
		return ( "[object Date]" === Object.prototype.toString.call( oObj ) && ( ! isNaN( oObj.getTime() ) ) );
	}

	/**
	 *	check if an object has own the specified properties
	 *
	 *	@param	oObj	{object}
	 *	@param	vKey	{array|string|numeric|undefined}
	 *	@return {boolean}
	 */
	static isObjectWithKeys( oObj, vKey )
	{
		let bRet;
		let vKeyKey;

		//	...
		bRet = false;

		if ( this.isObject( oObj ) )
		{
			if ( Array.isArray( vKey ) )
			{
				bRet = true;
				for ( vKeyKey in vKey )
				{
					if ( ! oObj.hasOwnProperty( vKey[ vKeyKey ] ) )
					{
						bRet = false;
						break;
					}
				}
			}
			else if ( this.isString( vKey ) || this.isNumeric( vKey ) )
			{
				bRet = oObj.hasOwnProperty( vKey );
			}
			else if ( undefined === vKey )
			{
				bRet = true;
			}
		}

		return bRet;
	}
}




/**
 *	@exports
 *	@type {Utilities}
 */
module.exports		= Utilities;