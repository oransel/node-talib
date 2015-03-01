/*
 
               |        |         |    o|    
 ,---.,---.,---|,---.   |--- ,---.|    .|---.
 |   ||   ||   ||---'---|    ,---||    ||   |
 `   '`---'`---'`---'   `---'`---^`---'``---'
 
 NODE-TALIB Copyright (c) 2012-2015, Mustafa Oransel
 All rights reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the
 Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.
 
*/ 

#include <node.h>
#include <nan.h>

#include "./lib/include/ta_abstract.h"
#include "./lib/include/ta_common.h"
#include "./lib/include/ta_defs.h"
#include "./lib/include/ta_func.h"

using namespace v8;

// Async work object
struct work_object {
    TA_FuncHandle *func_handle;
    int startIdx;
    int endIdx;
    int nbOutput;
    TA_ParamHolder *func_params;
    TA_RetCode retCode;
    int outBegIdx;
    int outNBElement;
    double **outReal;
    int **outInt;
};

static void REPORT_TA_ERROR(NanCallback *callback, TA_RetCode retCode) {

    // Report TA Error
    Local<Value> argv[1];
    Local<Object> result = NanNew<Object>();
    TA_RetCodeInfo retCodeInfo;
    TA_SetRetCodeInfo(retCode, &retCodeInfo);
    result->Set(NanNew<String>("error"), NanNew<String>(retCodeInfo.enumStr));
    argv[0] = result;
    callback->Call(1, argv);
}

static void REPORT_INTERNAL_ERROR(NanCallback *callback, const char *error) {

    // Report Internal Error
    Local<Value> argv[1];
    Local<Object> result = NanNew<Object>();
    result->Set(NanNew<String>("error"), NanNew<String>(error));
    argv[0] = result;
    callback->Call(1, argv);
}

static double *V8_TO_DOUBLE_ARRAY(Local<Array> array) {
        
    // Get the array length
    int length = array->Length();
    
    // Allocate memory for double array
    double *result = new double[length];
    
    // Store values in the double array
    for (int i = 0; i < length; i++) { 
        result[i] = array->Get(i)->NumberValue();
    }
    
    // Return the double array result
    return result;
    
}

static Handle<Value> TA_EXPLAIN_FUNCTION(const char *func_name) {
        
    // Function flag counter
    int func_param_flag_count;

    // Function handle and information
    const TA_FuncHandle *func_handle;
    const TA_FuncInfo   *func_info;
    
    // Function object
    Local<Object> func_object;
    Local<Object> func_param_object;
    Local<Object> func_param_flags;
    
    // Execution parameters
    Local<Array> inParams;
    Local<Array> inOptParams;
    Local<Array> outParams;
    
    // Function parameter information
    const TA_InputParameterInfo     *input_paraminfo;
    const TA_OptInputParameterInfo  *opt_paraminfo;
    const TA_OutputParameterInfo    *output_paraminfo;
    
    // Retreive the function information for the function handle
    TA_RetCode handle_retcode = TA_GetFuncHandle(func_name, &func_handle);
    TA_RetCode info_retcode = TA_GetFuncInfo(func_handle, &func_info);
    
    // Create the function object
    func_object = NanNew<Object>();
    
    // Check for error
    if ((handle_retcode != TA_SUCCESS) || (info_retcode != TA_SUCCESS))
        return func_object;
    
    // Create the execution parameters
    inParams    = NanNew<Array>();
    inOptParams = NanNew<Array>();
    outParams   = NanNew<Array>();
    
    // Store the function information
    func_object->Set(NanNew("name"), NanNew(func_info->name));
    func_object->Set(NanNew("group"), NanNew(func_info->group));
    func_object->Set(NanNew("hint"), NanNew(func_info->hint));
    
    // Loop for all the input parameters
    for (int i=0; i < (int)func_info->nbInput; i++) {
        
        // Get the function input parameter information
        TA_GetInputParameterInfo(func_info->handle, i, &input_paraminfo);
        
        // Create the function parameter information
        func_param_object = NanNew<Object>();
        func_param_object->Set(NanNew("name"), NanNew(input_paraminfo->paramName));

        // Add the function parameter type
        switch(input_paraminfo->type) {
            case TA_Input_Price: func_param_object->Set(NanNew("type"), NanNew("price")); break;
            case TA_Input_Real: func_param_object->Set(NanNew("type"), NanNew("real")); break;
            case TA_Input_Integer: func_param_object->Set(NanNew("type"), NanNew("integer")); break;
        }

        // Add the function parameter flags
        if (input_paraminfo->flags > 0) {

            // Create a new function flags array
            func_param_flags = NanNew<Object>();
            func_param_flag_count = 0;

            // Set the function flag defitions
            if (input_paraminfo->flags & TA_IN_PRICE_OPEN) func_param_flags->Set(func_param_flag_count++, NanNew("open"));
            if (input_paraminfo->flags & TA_IN_PRICE_HIGH) func_param_flags->Set(func_param_flag_count++, NanNew("high"));
            if (input_paraminfo->flags & TA_IN_PRICE_LOW) func_param_flags->Set(func_param_flag_count++, NanNew("low"));
            if (input_paraminfo->flags & TA_IN_PRICE_CLOSE) func_param_flags->Set(func_param_flag_count++, NanNew("close"));
            if (input_paraminfo->flags & TA_IN_PRICE_VOLUME) func_param_flags->Set(func_param_flag_count++, NanNew("volume"));
            if (input_paraminfo->flags & TA_IN_PRICE_OPENINTEREST) func_param_flags->Set(func_param_flag_count++, NanNew("openinterest"));
            if (input_paraminfo->flags & TA_IN_PRICE_TIMESTAMP) func_param_flags->Set(func_param_flag_count++, NanNew("timestamp"));
            
            // Save the function flag definitions
            func_param_object->Set(NanNew("flags"), func_param_flags);
        }
        
        // Save the function parameter
        inParams->Set(i, func_param_object);
        
    }
    
    // Loop for all the optional input parameters
    for (int i=0; i < (int)func_info->nbOptInput; i++) {
        
        // Get the function input parameter information
        TA_GetOptInputParameterInfo(func_info->handle, i, &opt_paraminfo);

        // Create the function parameter information
        func_param_object = NanNew<Object>();
        func_param_object->Set(NanNew("name"), NanNew(opt_paraminfo->paramName));
        func_param_object->Set(NanNew("displayName"), NanNew(opt_paraminfo->displayName));
        func_param_object->Set(NanNew("defaultValue"), NanNew(opt_paraminfo->defaultValue));
        func_param_object->Set(NanNew("hint"), NanNew(opt_paraminfo->hint));

        // Add the function parameter type
        switch(opt_paraminfo->type) {
            case TA_OptInput_RealRange: func_param_object->Set(NanNew("type"), NanNew("real_range")); break;
            case TA_OptInput_RealList: func_param_object->Set(NanNew("type"), NanNew("real_list")); break;
            case TA_OptInput_IntegerRange: func_param_object->Set(NanNew("type"), NanNew("integer_range")); break;
            case TA_OptInput_IntegerList: func_param_object->Set(NanNew("type"), NanNew("integer_list")); break;
        }

        // Add the function parameter flags
        if (opt_paraminfo->flags > 0) {

            // Create a new function flags array
            func_param_flags = NanNew<Object>();
            func_param_flag_count = 0;

            // Set the function flag defitions
            if (opt_paraminfo->flags & TA_OPTIN_IS_PERCENT) func_param_flags->Set(func_param_flag_count++, NanNew("percent"));
            if (opt_paraminfo->flags & TA_OPTIN_IS_DEGREE) func_param_flags->Set(func_param_flag_count++, NanNew("degree"));
            if (opt_paraminfo->flags & TA_OPTIN_IS_CURRENCY) func_param_flags->Set(func_param_flag_count++, NanNew("currency"));
            if (opt_paraminfo->flags & TA_OPTIN_ADVANCED) func_param_flags->Set(func_param_flag_count++, NanNew("advanced"));
            
            // Save the function flag definitions
            func_param_object->Set(NanNew("flags"), func_param_flags);
        }
        
        // Save the function parameter
        inOptParams->Set(i, func_param_object);
        
    }
    
    // Loop for all the output parameters
    for (int i=0; i < (int)func_info->nbOutput; i++) {
        
        // Get the function input parameter information
        TA_GetOutputParameterInfo(func_info->handle, i, &output_paraminfo);

        // Create the function parameter information
        func_param_object = NanNew<Object>();
        func_param_object->Set(NanNew("name"), NanNew(output_paraminfo->paramName));

        // Add the function parameter type
        switch(output_paraminfo->type) {
            case TA_Output_Real: func_param_object->Set(NanNew("type"), NanNew("real")); break;
            case TA_Output_Integer: func_param_object->Set(NanNew("type"), NanNew("integer")); break;
        }

        // Add the function parameter flags
        if (output_paraminfo->flags > 0) {

            // Create a new function flags array
            func_param_flags = NanNew<Object>();
            func_param_flag_count = 0;

            // Set the function flag defitions
            if (output_paraminfo->flags & TA_OUT_LINE) func_param_flags->Set(func_param_flag_count++, NanNew("line"));
            if (output_paraminfo->flags & TA_OUT_DOT_LINE) func_param_flags->Set(func_param_flag_count++, NanNew("line_dot"));
            if (output_paraminfo->flags & TA_OUT_DASH_LINE) func_param_flags->Set(func_param_flag_count++, NanNew("line_dash"));
            if (output_paraminfo->flags & TA_OUT_DOT) func_param_flags->Set(func_param_flag_count++, NanNew("dot"));
            if (output_paraminfo->flags & TA_OUT_HISTO) func_param_flags->Set(func_param_flag_count++, NanNew("histogram"));
            if (output_paraminfo->flags & TA_OUT_PATTERN_BOOL) func_param_flags->Set(func_param_flag_count++, NanNew("pattern_bool"));
            if (output_paraminfo->flags & TA_OUT_PATTERN_BULL_BEAR) func_param_flags->Set(func_param_flag_count++, NanNew("pattern_bull_bear"));
            if (output_paraminfo->flags & TA_OUT_PATTERN_STRENGTH) func_param_flags->Set(func_param_flag_count++, NanNew("pattern_strength"));
            if (output_paraminfo->flags & TA_OUT_POSITIVE) func_param_flags->Set(func_param_flag_count++, NanNew("positive"));
            if (output_paraminfo->flags & TA_OUT_NEGATIVE) func_param_flags->Set(func_param_flag_count++, NanNew("negative"));
            if (output_paraminfo->flags & TA_OUT_ZERO) func_param_flags->Set(func_param_flag_count++, NanNew("zero"));
            if (output_paraminfo->flags & TA_OUT_UPPER_LIMIT) func_param_flags->Set(func_param_flag_count++, NanNew("limit_upper"));
            if (output_paraminfo->flags & TA_OUT_LOWER_LIMIT) func_param_flags->Set(func_param_flag_count++, NanNew("limit_lower"));
            
            // Save the function flag definitions
            func_param_object->Set(NanNew("flags"), func_param_flags);
        }
        
        // Save the function parameter
        outParams->Set(i, func_param_object);
        
    }
    
    // Store the function parameters
    func_object->Set(NanNew("inputs"), inParams);
    func_object->Set(NanNew("optInputs"), inOptParams);
    func_object->Set(NanNew("outputs"), outParams);
    
    return func_object;
    
}

NAN_GETTER(Functions) {
    NanScope();

    // Function group table
    TA_StringTable *group_table;
    
    // Function table
    TA_StringTable *func_table;

    // Function name array
    Local<Array> func_array = NanNew<Array>();

    // Function count
    int func_count = 0;

    // Get the function groups
    if (TA_GroupTableAlloc(&group_table) == TA_SUCCESS) {
        
        // Loop for all the function groups
        for (unsigned int group_index=0; group_index < group_table->size; group_index++) {
            
            // Get the functions in the group
            if (TA_FuncTableAlloc(group_table->string[group_index], &func_table) == TA_SUCCESS) {
                
                // Loop for all the functions
                for (unsigned int func_index=0; func_index < func_table->size; func_index++) {
                    
                    // Save the function name to the array
                    func_array->Set(func_count++, TA_EXPLAIN_FUNCTION(func_table->string[func_index]));
                    
                }
                
                // Clear function table memory
                TA_FuncTableFree(func_table);
            }
            
        }
        
        // Clear function group table memory
        TA_GroupTableFree(group_table);
        
    }

    // Return function names
    NanReturnValue(func_array);
}

NAN_METHOD(Explain) {
    NanScope();

    // Check if any arguments are passed
    if (args.Length() < 1) {
        NanThrowTypeError("One argument required - Function name");
        NanReturnUndefined();
    }

    // Check if first parameter is a string
    if (!args[0]->IsString()) {
        NanThrowTypeError("First argument must be a String");
        NanReturnUndefined();
    }

    // Retreive the function name string
    NanAsciiString func_name(args[0]->ToString());
    
    NanReturnValue(TA_EXPLAIN_FUNCTION(*func_name));
}

class ExecuteWorker : public NanAsyncWorker {
 public:
  ExecuteWorker(NanCallback *callback, work_object *wo): NanAsyncWorker(callback), wo(wo) {}
  ~ExecuteWorker() {

    // Clear parameter holder memory
    TA_ParamHolderFree(wo->func_params);
    
    // Dispose output arrays
    for (int i=0; i < wo->nbOutput; i++) {
        delete wo->outReal[i];
        delete wo->outInt[i];
    }
    delete wo->outReal;
    delete wo->outInt;
    
    // Dispose work object
    delete wo;

  }

  void Execute () {

    // Execute the function call with parameters declared
    wo->retCode = TA_CallFunc((const TA_ParamHolder *)wo->func_params, wo->startIdx, wo->endIdx, &wo->outBegIdx, &wo->outNBElement);

  }

  void HandleOKCallback () {
    NanScope();

    // Create the outputs object
    Local<Object> outputArray = NanNew<Object>();
    
    // Execution result object 
    Local<Object> result = NanNew<Object>();
    
    // Result args
    Local<Value> argv[1];
    
    // Determine the number of results
    int resultLength = wo->outNBElement;
    
    // Function output parameter information
    const TA_OutputParameterInfo *output_paraminfo;
    
    // Check for execution error
    if (wo->retCode != TA_SUCCESS) {
        return REPORT_TA_ERROR(callback, wo->retCode);
    }
    
    // Set beginning index and number of elements
    result->Set(NanNew<String>("begIndex"), NanNew<Number>(wo->outBegIdx));
    result->Set(NanNew<String>("nbElement"), NanNew<Number>(wo->outNBElement));
    
    // Loop for all the output parameters
    for (int i=0; i < wo->nbOutput; i++) {
        
        // Get the output parameter information
        TA_GetOutputParameterInfo(wo->func_handle, i, &output_paraminfo);
        
        // Create an array for results
        Local<Array> resultArray = NanNew<Array>(resultLength);
        
        // Loop for all the results
        for (int x = 0; x < resultLength; x++) {
            
            // Determine the output type
            switch(output_paraminfo->type) {
                    
                    // Output type real is needed
                case TA_Output_Real:
                    
                    // Set the real output value
                    resultArray->Set(x, NanNew<Number>(wo->outReal[i][x]));
                    
                    break;
                    
                    // Output type integer is needed
                case TA_Output_Integer:
                    
                    // Set the integer output value
                    resultArray->Set(x, NanNew<Number>(wo->outInt[i][x]));
                    
                    break;
            }
            
        }
        
        // Set the result array
        outputArray->Set(NanNew<String>(output_paraminfo->paramName), resultArray);
        
    }
    
    // Set the outputs array
    result->Set(NanNew<String>("result"), outputArray);
    
    // Return the execution result
    argv[0] = result;
    callback->Call(1, argv);

  };

 private:
  work_object *wo;
};

NAN_METHOD(Execute) {
    NanScope();

    // Execution parameter
    Local<Object> executeParameter;

    // Callback function
    NanCallback *cb;

    // Price values
    double *open            = NULL;
    double *close           = NULL;
    double *high            = NULL;
    double *low             = NULL;
    double *volume          = NULL;
    double *openInterest    = NULL;
    
    double *inRealList      = NULL;
    
    // Numeric values
    double inReal;
    int inInteger;
    
    // Return code
    TA_RetCode retCode;
    
    // Function handle
    const TA_FuncHandle *func_handle;
    
    // Function information
    const TA_FuncInfo *func_info;
    
    // Function parameter holder
    TA_ParamHolder *func_params;
    
    // Function parameter information
    const TA_InputParameterInfo     *input_paraminfo;
    const TA_OptInputParameterInfo  *opt_paraminfo;
    const TA_OutputParameterInfo    *output_paraminfo;

    // Check the arguments
    if (args.Length() < 2) {
        NanThrowTypeError("Two arguments required - Object and Function");
        NanReturnUndefined();
    }

    // Check the execution parameter
    if (!args[0]->IsObject()) {
        NanThrowTypeError("First argument must be an Object with fields name, startIdx, endIdx and function input parameters");
        NanReturnUndefined();
    }
    
    // Check the callback parameter
    if (!args[1]->IsFunction()) {
        NanThrowTypeError("Second argument must be a Function");
        NanReturnUndefined();
    }
    
    // Get the execute parameter
    executeParameter = args[0]->ToObject();
    
    // Get the callback function
    cb = new NanCallback(args[1].As<Function>());
    
    // Check the function name parameter
    if (!executeParameter->HasOwnProperty(NanNew<String>("name"))) {
        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'name' field");
        NanReturnUndefined();
    }
    
    // Retreive the function name string
    NanAsciiString func_name(executeParameter->Get(NanNew<String>("name")));
    
    // Check the start index
    if (!executeParameter->HasOwnProperty(NanNew<String>("startIdx"))) {
        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'startIdx' field");
        NanReturnUndefined();
    }
    
    // Check the end index
    if (!executeParameter->HasOwnProperty(NanNew<String>("endIdx"))) {
        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'endIdx' field");
        NanReturnUndefined();
    }
    
    // Refreive the start and end index
    int startIdx = executeParameter->Get(NanNew<String>("startIdx"))->Int32Value();
    int endIdx = executeParameter->Get(NanNew<String>("endIdx"))->Int32Value();

    // Check for negative indexes
    if ((startIdx < 0) || (endIdx < 0)) {
        REPORT_INTERNAL_ERROR(cb, "Arguments 'startIdx' and 'endIdx' need to be positive");
        NanReturnUndefined();
    }

    // Check for index correctness
    if (startIdx > endIdx) {
        REPORT_INTERNAL_ERROR(cb, "Argument 'startIdx' needs to be smaller than argument 'endIdx'");
        NanReturnUndefined();
    }

    // Retreive the function handle for function name
    if ((retCode = TA_GetFuncHandle(*func_name, &func_handle)) != TA_SUCCESS) {
        REPORT_TA_ERROR(cb, retCode);
        NanReturnUndefined();
    }
    
    // Retreive the function information for the function handle
    if ((retCode = TA_GetFuncInfo(func_handle, &func_info)) != TA_SUCCESS) {
        REPORT_TA_ERROR(cb, retCode);
        NanReturnUndefined();
    }

    // Allocate parameter holder memory for function handle
    if ((retCode = TA_ParamHolderAlloc(func_handle, &func_params)) != TA_SUCCESS) {
        REPORT_TA_ERROR(cb, retCode);
        NanReturnUndefined();
    }

    // Loop for all the input parameters
    for (int i=0; i < (int)func_info->nbInput; i++) {

        // Get the function input parameter information
        TA_GetInputParameterInfo(func_info->handle, i, &input_paraminfo);

        // Check the input parameter type
        switch (input_paraminfo->type) {

            // Input parameter price type is needed
            case TA_Input_Price:
                
                // Check if the parameter info requires open price
                if (input_paraminfo->flags & TA_IN_PRICE_OPEN) {
                    
                    // Check if the price object has open prices
                    if (!executeParameter->HasOwnProperty(NanNew<String>("open"))) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'open' field");
                        NanReturnUndefined();
                        
                    }
                    
                    // Get the open prices
                    open = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(executeParameter->Get(NanNew<String>("open"))));
                    
                }
                
                // Check if the parameter info requires high price
                if (input_paraminfo->flags & TA_IN_PRICE_HIGH) {
                    
                    // Check if the price object has high prices
                    if (!executeParameter->HasOwnProperty(NanNew<String>("high"))) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);

                        // Clear values memory
                        if (open)
                            delete[] open;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'high' field");
                        NanReturnUndefined();
                        
                    }
                    
                    // Get the high prices
                    high = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(executeParameter->Get(NanNew<String>("high"))));
                    
                }
                
                // Check if the parameter info requires low price
                if (input_paraminfo->flags & TA_IN_PRICE_LOW) {
                    
                    // Check if the price object has low prices
                    if (!executeParameter->HasOwnProperty(NanNew<String>("low"))) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Clear values memory
                        if (open)
                            delete[] open;
                        if (high)
                            delete[] high;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'low' field");
                        NanReturnUndefined();
                        
                    }
                    
                    // Get the low prices
                    low = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(executeParameter->Get(NanNew<String>("low"))));
                    
                }
                
                // Check if the parameter info requires close price
                if (input_paraminfo->flags & TA_IN_PRICE_CLOSE) {
                    
                    // Check if the price object has close prices
                    if (!executeParameter->HasOwnProperty(NanNew<String>("close"))) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Clear values memory
                        if (open)
                            delete[] open;
                        if (low)
                            delete[] low;
                        if (high)
                            delete[] high;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'close' field");
                        NanReturnUndefined();
                        
                    }
                    
                    // Get the close prices
                    close = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(executeParameter->Get(NanNew<String>("close"))));
                    
                }
                
                // Check if the parameter info requires volume
                if (input_paraminfo->flags & TA_IN_PRICE_VOLUME) {
                    
                    // Check if the price object has volume
                    if (!executeParameter->HasOwnProperty(NanNew<String>("volume"))) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);

                        // Clear values memory
                        if (open)
                            delete[] open;
                        if (close)
                            delete[] close;
                        if (low)
                            delete[] low;
                        if (high)
                            delete[] high;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'volume' field");
                        NanReturnUndefined();
                        
                    }
                    
                    // Get the volume
                    volume = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(executeParameter->Get(NanNew<String>("volume"))));
                    
                }
                
                // Check if the parameter info requires open interest values
                if (input_paraminfo->flags & TA_IN_PRICE_OPENINTEREST) {
                    
                    // Check if the price object has open interest values
                    if (!executeParameter->HasOwnProperty(NanNew<String>("openInterest"))) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Clear values memory
                        if (close)
                            delete[] close;
                        if (volume)
                            delete[] volume;
                        if (high)
                            delete[] high;
                        if (low)
                            delete[] low;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'openInterest' field");
                        NanReturnUndefined();
                        
                    }
                    
                    // Get the open interest values
                    openInterest = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(executeParameter->Get(NanNew<String>("openInterest"))));
                    
                }
                
                // Save the price parameters
                if ((retCode = TA_SetInputParamPricePtr(func_params, i, open, high, low, close, volume, openInterest)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    NanReturnUndefined();
                    
                }
                
                break;

            // Input parameter real type is needed
            case TA_Input_Real:
                
                // Check if the input parameter object has real value
                if (!executeParameter->HasOwnProperty(NanNew<String>(input_paraminfo->paramName))) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return internal error
                    REPORT_INTERNAL_ERROR(cb, ((std::string)("First argument must contain '") + (std::string)(input_paraminfo->paramName) + (std::string)("' field")).c_str());
                    NanReturnUndefined();
                    
                }
                
                // Get the number parameter value
                inRealList = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(executeParameter->Get(NanNew<String>(input_paraminfo->paramName))));
                 
                // Save the number parameter
                if ((retCode = TA_SetInputParamRealPtr(func_params, i, inRealList)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    NanReturnUndefined();
                    
                }
                
                break;
            
            // Input parameter integer type is needed
            case TA_Input_Integer:
                
                // Check if the input parameter object has integer parameter
                if (!executeParameter->HasOwnProperty(NanNew<String>(input_paraminfo->paramName))) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return internal error
                    REPORT_INTERNAL_ERROR(cb, ((std::string)("First argument must contain '") + (std::string)(input_paraminfo->paramName) + (std::string)("' field")).c_str());
                    NanReturnUndefined();

                }
                
                // Get the integer parameter value
                inInteger = executeParameter->Get(NanNew<String>(input_paraminfo->paramName))->IntegerValue();
                
                // Save the integer parameter
                if ((retCode = TA_SetInputParamIntegerPtr(func_params, i, &inInteger)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    NanReturnUndefined();
                    
                }
                
                break;

        }

    }

    // Loop for all the optional input parameters
    for (int i=0; i < (int)func_info->nbOptInput; i++) {
        
        // Get the optional input parameter information
        TA_GetOptInputParameterInfo(func_info->handle, i, &opt_paraminfo);
        
        // Check if the optional parameter object has real value
        if (!executeParameter->HasOwnProperty(NanNew<String>(opt_paraminfo->paramName))) {
            
            // Clear parameter holder memory
            TA_ParamHolderFree(func_params);
            
            // Return internal error
            REPORT_INTERNAL_ERROR(cb, ((std::string)("First argument must contain '") + (std::string)(opt_paraminfo->paramName) + (std::string)("' field")).c_str());
            NanReturnUndefined();

        }
        
        // Check the optional input parameter type
        switch (opt_paraminfo->type) {
                
            case TA_OptInput_RealRange:
            case TA_OptInput_RealList:
                
                // Get the integer parameter value
                inReal = executeParameter->Get(NanNew<String>(opt_paraminfo->paramName))->NumberValue();
                
                // Save the integer parameter
                if ((retCode = TA_SetOptInputParamReal(func_params, i, inReal)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    NanReturnUndefined();
                    
                }
                
                break;
            case TA_OptInput_IntegerRange:
            case TA_OptInput_IntegerList:
                
                // Get the integer parameter value
                inInteger = executeParameter->Get(NanNew<String>(opt_paraminfo->paramName))->IntegerValue();
                
                // Save the integer parameter
                if ((retCode = TA_SetOptInputParamInteger(func_params, i, inInteger)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    NanReturnUndefined();
                    
                }
                
                break;
                
        }
        
    }

    // Setup the work object
    work_object *wo = new work_object();
    wo->func_handle = (TA_FuncHandle *)func_handle;
    wo->startIdx = startIdx;
    wo->endIdx = endIdx;
    wo->nbOutput = func_info->nbOutput;
    wo->func_params = func_params;
    wo->outReal = new double*[func_info->nbOutput];
    wo->outInt = new int*[func_info->nbOutput];
    
    // Loop for all the ouput parameters
    for (int i=0; i < wo->nbOutput; i++) {
        
        // Allocate memory
        wo->outReal[i] = new double[endIdx-startIdx+1];
        wo->outInt[i] = new int[endIdx-startIdx+1];
        
        // Clear output results memory
        for (int x=0; x < (endIdx-startIdx+1); x++) {
            wo->outReal[i][x] = 0;
            wo->outInt[i][x] = 0;
        }
        
        // Get the output parameter information
        TA_GetOutputParameterInfo(func_info->handle, i, &output_paraminfo);
        
        // Check the output parameter type
        switch(output_paraminfo->type) {
            
            // Output parameter type real is needed
            case TA_Output_Real:
                
                // Assign the real parameter memory
                TA_SetOutputParamRealPtr(func_params, i, &wo->outReal[i][0]);
                
                break;
            
            // Output parameter type integer is needed
            case TA_Output_Integer:
                
                // Assign the integer parameter memory
                TA_SetOutputParamIntegerPtr(func_params, i, &wo->outInt[i][0]);
                
                break;
                
        }
        
    }

    // Queue the work
    NanAsyncQueueWorker(new ExecuteWorker(cb, wo));
    NanReturnUndefined();
}

void Init(Handle<Object> exports, Handle<Object> module) {

    // Initialize the engine
    TA_Initialize();

    // Define fields
    exports->Set(NanNew("version"), NanNew("0.5.0"));

    // Define accessors
    exports->SetAccessor(NanNew("functions"), Functions);

    // Define functions
    exports->Set(NanNew("explain"), NanNew<FunctionTemplate>(Explain)->GetFunction());
    exports->Set(NanNew("execute"), NanNew<FunctionTemplate>(Execute)->GetFunction());
}

NODE_MODULE(talib, Init)
