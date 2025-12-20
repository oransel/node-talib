/**
 * node-talib - Technical Analysis Library
 * TypeScript definitions
 */

declare module 'talib' {
  /**
   * TA-Lib version string
   */
  export const version: string;

  /**
   * Moving Average Types
   */
  export enum MAType {
    SMA = 0,   // Simple Moving Average
    EMA = 1,   // Exponential Moving Average
    WMA = 2,   // Weighted Moving Average
    DEMA = 3,  // Double Exponential Moving Average
    TEMA = 4,  // Triple Exponential Moving Average
    TRIMA = 5, // Triangular Moving Average
    KAMA = 6,  // Kaufman Adaptive Moving Average
    MAMA = 7,  // MESA Adaptive Moving Average
    T3 = 8     // Triple Exponential Moving Average (T3)
  }

  /**
   * Function Unstable Period IDs
   */
  export enum FunctionUnstId {
    TA_FUNC_UNST_ADX = 0,
    TA_FUNC_UNST_ADXR = 1,
    TA_FUNC_UNST_ATR = 2,
    TA_FUNC_UNST_CMO = 3,
    TA_FUNC_UNST_DX = 4,
    TA_FUNC_UNST_EMA = 5,
    TA_FUNC_UNST_HT_DCPERIOD = 6,
    TA_FUNC_UNST_HT_DCPHASE = 7,
    TA_FUNC_UNST_HT_PHASOR = 8,
    TA_FUNC_UNST_HT_SINE = 9,
    TA_FUNC_UNST_HT_TRENDLINE = 10,
    TA_FUNC_UNST_HT_TRENDMODE = 11,
    TA_FUNC_UNST_IMI = 12,
    TA_FUNC_UNST_KAMA = 13,
    TA_FUNC_UNST_MAMA = 14,
    TA_FUNC_UNST_MFI = 15,
    TA_FUNC_UNST_MINUS_DI = 16,
    TA_FUNC_UNST_MINUS_DM = 17,
    TA_FUNC_UNST_NATR = 18,
    TA_FUNC_UNST_PLUS_DI = 19,
    TA_FUNC_UNST_PLUS_DM = 20,
    TA_FUNC_UNST_RSI = 21,
    TA_FUNC_UNST_STOCHRSI = 22,
    TA_FUNC_UNST_T3 = 23,
    TA_FUNC_UNST_ALL = 24,
    TA_FUNC_UNST_NONE = -1
  }

  /**
   * Market data input
   */
  export interface MarketData {
    open?: number[];
    high?: number[];
    low?: number[];
    close?: number[];
    volume?: number[];
    openInterest?: number[];
  }

  /**
   * Function input parameter info
   */
  export interface InputParameterInfo {
    name: string;
    type: 'price' | 'real' | 'integer';
    flags?: string[];
  }

  /**
   * Function optional input parameter info
   */
  export interface OptInputParameterInfo {
    name: string;
    displayName: string;
    defaultValue: number;
    hint: string;
    type: 'real_range' | 'real_list' | 'integer_range' | 'integer_list';
    flags?: string[];
  }

  /**
   * Function output parameter info
   */
  export interface OutputParameterInfo {
    name: string;
    type: 'real' | 'integer';
    flags?: string[];
  }

  /**
   * Function explanation
   */
  export interface FunctionExplanation {
    name: string;
    group: string;
    hint: string;
    inputs: InputParameterInfo[];
    optInputs: OptInputParameterInfo[];
    outputs: OutputParameterInfo[];
  }

  /**
   * Execute parameters
   */
  export interface ExecuteParameters extends MarketData {
    name: string;
    startIdx: number;
    endIdx: number;
    [key: string]: any; // For optional parameters like optInTimePeriod
  }

  /**
   * Execute result
   */
  export interface ExecuteResult {
    begIndex: number;
    nbElement: number;
    result: {
      [key: string]: number[];
    };
  }

  /**
   * Execute error result
   */
  export interface ExecuteError {
    error: string;
  }

  /**
   * Array of all available functions
   */
  export const functions: FunctionExplanation[];

  /**
   * Object containing function unstable period IDs
   */
  export const functionUnstIds: Record<string, number>;

  /**
   * Explain a function's parameters and usage
   * @param functionName - Name of the TA-Lib function
   * @returns Function explanation object
   */
  export function explain(functionName: string): FunctionExplanation;

  /**
   * Execute a TA-Lib function (async with callback)
   * @param params - Execution parameters
   * @param callback - Callback function
   */
  export function execute(
    params: ExecuteParameters,
    callback: (err: ExecuteError | null, result?: ExecuteResult) => void
  ): void;

  /**
   * Execute a TA-Lib function (synchronous)
   * @param params - Execution parameters
   * @returns Execution result
   */
  export function execute(params: ExecuteParameters): ExecuteResult;

  /**
   * Set unstable period for a function
   * @param functionUnstId - Function unstable ID
   * @param unstablePeriod - Unstable period value
   * @returns Success boolean
   */
  export function setUnstablePeriod(
    functionUnstId: number,
    unstablePeriod: number
  ): boolean;
}

export = talib;
