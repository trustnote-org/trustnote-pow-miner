const _child_process	= require( 'child_process' );
const _children		= require( '../js/children.js' );


const child		= _child_process.exec( "node -e 'while (true);'", function (){} );
_children( process.pid, function( err, arrChildren )
{
	console.log( err, arrChildren );
});