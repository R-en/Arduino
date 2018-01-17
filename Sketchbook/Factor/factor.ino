long int v;
  
void setup( )
  {
  Serial.begin( 115200 );
  Serial.println( "Please enter a number to factored." );
  v = 0;
  }

void PrintNumber( long int i )
  {
  char buffer[20];
  sprintf( buffer, ( char * )"%ld", i );
  Serial.println( buffer );
  }
  
long int factor( long int n )
   {
   if ( n > 3 )
      {
      long int i;
      for ( i = 2;  i*i <= n;  i++ )
         if ( n % i == 0 )
            {
            PrintNumber( i );
            return factor( n / i );
            }
      }
   return n;
   }

void loop( )
  {
  int c;
  if ( ( c = Serial.read( ) ) != -1 )
    {
    Serial.write( c );
    if ( c >= '0' && c <= '9')
      v = v*10 + ( c - '0' );
    else
      if ( c == '\r' )
         {
         Serial.write( '\n' );
         Serial.println( "Factoring ..." );
         PrintNumber( factor( v ) );
         Serial.println( "Please enter a number to factored." );
         v = 0;
         }
   }
 }
