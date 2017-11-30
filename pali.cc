#include <node.h>
#include <stdlib.h>
#include <math.h>

namespace demo {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Number;

// -----------------------
// long get_pali(int digits);

typedef struct {
	int *array;
	int len;
} Array;

typedef struct {
	long int prod;
	int f1, f2;
} Result;

// typedef struct array_struct Array;
// typedef struct result_struct Result;

// void print_time_spent(clock_t clocks, char *str);
char is_pali(long int num);

Array *get_primes(int max);
Result *get_pali(Array *primes);

Array *get_primes(int digits)
{
	int *sieve, *primes;
	int primes_len = 0;

	int max = pow(10, digits);
	int min = pow(10, digits - 1);

	Array *primes_s = (Array *) malloc(sizeof(Array));

	sieve = (int *) calloc(max + 1, sizeof(int));
	primes = NULL;

	for (int i = 2; i <= max; i++) {
		if ( !sieve[i] ) {
			if ( i >= min ) {
				primes_len++;
				primes = (int *) realloc(primes, primes_len * sizeof(int));
				primes[primes_len - 1] = i;
			}
			for (int j = i << 1; j <= max; j += i) {
				sieve[j] = 1;
			}
		}
	}

	free(sieve);

	primes_s->array = primes;
	primes_s->len = primes_len;

	return primes_s;
}

Result *get_pali(Array *primes)
{
	Result *result = (Result *) malloc(sizeof(Result));

	long int	prod 		= 0,
				max_pali 	= 0;
	int f1, f2;

	for (int i = 0; i < primes->len; i++) {
		for (int j = i; j < primes->len; j++) {
			prod = (long int) primes->array[i] * primes->array[j];
			if ( is_pali(prod) ) {
				if (prod > max_pali) {
					max_pali = prod;
					f1 = primes->array[i];
					f2 = primes->array[j];
				}
			}

		}
	}
	
	result->prod = max_pali;
	result->f1 = f1;
	result->f2 = f2;
  
	return result;
}

char is_pali(long int num)
{	
	long int rev = 0;
    long int buf = num;

	do {
		rev = rev * 10 + buf % 10;
	} while (buf /= 10);
	return rev == num;
}



// ------------------------

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed.
  if (args.Length() < 1) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  Array *primes;
  Result *result;

  // Perform the operation
  primes = get_primes(args[0]->NumberValue());
  result = get_pali(primes);

  Local<Number> prod = Number::New(isolate, result->prod);
  Local<Number> f1 = Number::New(isolate, result->f1);
  Local<Number> f2 = Number::New(isolate, result->f2);

  Local<Object> obj = Object::New(isolate);
  obj->Set(String::NewFromUtf8(isolate, "prod"), prod);
  obj->Set(String::NewFromUtf8(isolate, "f1"), f1);
  obj->Set(String::NewFromUtf8(isolate, "f2"), f2);

  args.GetReturnValue().Set(obj);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "get", Method);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init)

/*long get_pali(int digits = 4)
{
	return (long) digits * digits;
}
*/
}  // namespace demo
