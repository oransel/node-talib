/**
 * Chaikin Accumulation/Distribution Line.
 * Measures cumulative money flow by assessing whether a security is being accumulated (bought) or distributed (sold) based on price and volume.
 */
export const AD = 'AD' as const;

/**
 * Chaikin Accumulation/Distribution Oscillator.
 * Applies the MACD formula to the Chaikin A/D Line (subtracting a longer EMA from a shorter EMA) to measure momentum of volume flows.
 */
export const ADOSC = 'ADOSC' as const;

/**
 * Average Directional Movement Index.
 * Measures the overall strength of a price trend (from 0 to 100) regardless of whether the trend is upward or downward.
 */
export const ADX = 'ADX' as const;

/**
 * Average Directional Movement Index Rating.
 * The average of the current ADX value and an ADX value from a specified number of periods ago, used to smooth and evaluate trend momentum changes.
 */
export const ADXR = 'ADXR' as const;

/**
 * Absolute Price Oscillator.
 * Shows the difference between two moving averages (fast and slow) expressed as an absolute value to identify trend direction and crossovers.
 */
export const APO = 'APO' as const;

/**
 * Aroon.
 * Determines whether a security is trending or range-bound, and evaluates the strength of the trend by measuring the time elapsed since the highest high and lowest low.
 */
export const AROON = 'AROON' as const;

/**
 * Aroon Oscillator.
 * Calculated by subtracting Aroon Down from Aroon Up; values above 0 signal an uptrend, while values below 0 signal a downtrend.
 */
export const AROONOSC = 'AROONOSC' as const;

/**
 * Average True Range.
 * Measures market volatility by decomposing the entire range of an asset price for a given period.
 */
export const ATR = 'ATR' as const;

/**
 * Average Price.
 * Calculated as the simple average of the Open, High, Low, and Close prices for a given period.
 */
export const AVGPRICE = 'AVGPRICE' as const;

/**
 * Bollinger Bands.
 * A volatility indicator consisting of a simple moving average (middle band) and two standard deviation bands plotted above and below it.
 */
export const BBANDS = 'BBANDS' as const;

/**
 * Beta.
 * Measures the volatility or systematic risk of a security in comparison to the market as a whole.
 */
export const BETA = 'BETA' as const;

/**
 * Balance Of Power.
 * Evaluates the systematic strength of buyers (bulls) versus sellers (bears) by assessing the ability of the market to push prices to extreme closes.
 */
export const BOP = 'BOP' as const;

/**
 * Commodity Channel Index.
 * A momentum-based oscillator used to identify new trends, warn of extreme overbought/oversold conditions, or spot trend strength.
 */
export const CCI = 'CCI' as const;

/**
 * Two Crows.
 * A bearish candlestick reversal pattern consisting of a long green candle followed by two consecutive red candles.
 */
export const CDL2CROWS = 'CDL2CROWS' as const;

/**
 * Three Black Crows.
 * A bearish candlestick reversal pattern consisting of three consecutive long-bodied red candles that close near their lows.
 */
export const CDL3BLACKCROWS = 'CDL3BLACKCROWS' as const;

/**
 * Three Inside Up/Down.
 * A candlestick reversal pattern consisting of a large candle, followed by an inside candle of the opposite color, completed by a third confirming candle.
 */
export const CDL3INSIDE = 'CDL3INSIDE' as const;

/**
 * Three Outside Up/Down.
 * A candlestick reversal pattern consisting of a candle, followed by an engulfing candle of the opposite color, completed by a third confirming candle.
 */
export const CDL3LINESTRIKE = 'CDL3LINESTRIKE' as const;

/**
 * Three Stars In The South.
 * A rare bullish candlestick reversal pattern characterized by three consecutive red candles with progressively smaller bodies and higher lows.
 */
export const CDL3STARSINSOUTH = 'CDL3STARSINSOUTH' as const;

/**
 * Three Advancing White Soldiers.
 * A bullish candlestick reversal pattern consisting of three consecutive long-bodied green candles that close progressively higher.
 */
export const CDL3WHITESOLDIERS = 'CDL3WHITESOLDIERS' as const;

/**
 * Abandoned Baby.
 * A rare, highly reliable three-candle reversal pattern where a doji is completely separated by gaps from the surrounding candlesticks.
 */
export const CDLABANDONEDBABY = 'CDLABANDONEDBABY' as const;

/**
 * Advance Block.
 * A bearish candlestick warning pattern consisting of three green candles that show signs of weakening momentum through smaller bodies and long upper shadows.
 */
export const CDLADVANCEBLOCK = 'CDLADVANCEBLOCK' as const;

/**
 * Belt-hold.
 * A single candlestick pattern (bullish or bearish) where the opening price is the high/low of the day and the body closes near the opposite end.
 */
export const CDLBELTHOLD = 'CDLBELTHOLD' as const;

/**
 * Breakaway.
 * A five-candle reversal pattern that signals a trend reversal, characterized by a gap in the direction of the trend followed by counter-trend candles.
 */
export const CDLBREAKAWAY = 'CDLBREAKAWAY' as const;

/**
 * Closing Marubozu.
 * A candlestick pattern where the closing price matches either the absolute high (bullish) or absolute low (bearish) of the period, indicating strong closing control.
 */
export const CDLCLOSINGMARUBOZU = 'CDLCLOSINGMARUBOZU' as const;

/**
 * Concealing Baby Swallow.
 * A rare four-candle bullish reversal pattern appearing in a downtrend, featuring two consecutive marubozus, a gapped third candle, and a final engulfing candle.
 */
export const CDLCONCEALBABYSWALL = 'CDLCONCEALBABYSWALL' as const;

/**
 * Counterattack.
 * A two-candle reversal pattern where opposing candles of different colors end up closing at the exact same price level.
 */
export const CDLCOUNTERATTACK = 'CDLCOUNTERATTACK' as const;

/**
 * Dark Cloud Cover.
 * A bearish candlestick reversal pattern where a red candle opens above the high of a green candle but closes below its midpoint.
 */
export const CDLDARKCLOUDCOVER = 'CDLDARKCLOUDCOVER' as const;

/**
 * Doji.
 * A candlestick pattern formed when the opening and closing prices of a security are virtually equal, representing market indecision.
 */
export const CDLDOJI = 'CDLDOJI' as const;

/**
 * Doji Star.
 * A candlestick pattern featuring a doji that gaps above or below the body of the preceding candle, signaling a potential trend reversal.
 */
export const CDLDOJISTAR = 'CDLDOJISTAR' as const;

/**
 * Dragonfly Doji.
 * A doji candlestick pattern with a long lower shadow and no upper shadow, representing potential bullish reversal or support.
 */
export const CDLDRAGONFLYDOJI = 'CDLDRAGONFLYDOJI' as const;

/**
 * Engulfing Pattern.
 * A strong two-candle reversal pattern where the body of the second candle completely overlaps (engulfs) the body of the preceding candle.
 */
export const CDLENGULFING = 'CDLENGULFING' as const;

/**
 * Evening Doji Star.
 * A bearish three-candle reversal pattern consisting of a long green candle, a gapped doji, and a long red candle confirming the decline.
 */
export const CDLEVENINGDOJISTAR = 'CDLEVENINGDOJISTAR' as const;

/**
 * Evening Star.
 * A bearish three-candle reversal pattern consisting of a long green candle, a small-bodied star candle, and a long red candle confirming the decline.
 */
export const CDLEVENINGSTAR = 'CDLEVENINGSTAR' as const;

/**
 * Up/Down-gap side-by-side white lines.
 * A continuation candlestick pattern consisting of a gap followed by two side-by-side green candles of similar size.
 */
export const CDLGAPSIDESIDEWHITE = 'CDLGAPSIDESIDEWHITE' as const;

/**
 * Gravestone Doji.
 * A doji candlestick pattern with a long upper shadow and no lower shadow, representing potential bearish reversal or overhead resistance.
 */
export const CDLGRAVESTONEDOJI = 'CDLGRAVESTONEDOJI' as const;

/**
 * Hammer.
 * A bullish reversal candlestick pattern featuring a small upper body and a long lower shadow (at least twice the size of the body) that occurs in a downtrend.
 */
export const CDLHAMMER = 'CDLHAMMER' as const;

/**
 * Hanging Man.
 * A bearish reversal candlestick pattern featuring a small upper body and a long lower shadow that occurs at the peak of an uptrend.
 */
export const CDLHANGINGMAN = 'CDLHANGINGMAN' as const;

/**
 * Harami Pattern.
 * A two-candle pattern where a small candlestick is completely contained within the body of the preceding large candlestick, signaling a potential reversal.
 */
export const CDLHARAMI = 'CDLHARAMI' as const;

/**
 * Harami Cross Pattern.
 * A two-candle pattern where the second candlestick is a doji completely contained within the body of the preceding large candlestick.
 */
export const CDLHARAMICROSS = 'CDLHARAMICROSS' as const;

/**
 * High-Wave Candle.
 * A candlestick pattern with very long upper and lower shadows and a small body, representing extreme volatility and market confusion.
 */
export const CDLHIGHWAVE = 'CDLHIGHWAVE' as const;

/**
 * Hikkake Pattern.
 * A candlestick pattern used to identify potential false breakouts and subsequent market reversals.
 */
export const CDLHIKKAKE = 'CDLHIKKAKE' as const;

/**
 * Modified Hikkake Pattern.
 * An advanced variation of the standard Hikkake pattern that incorporates strict context requirements for identifying false breakouts.
 */
export const CDLHIKKAKEMOD = 'CDLHIKKAKEMOD' as const;

/**
 * Homing Pigeon.
 * A bullish reversal pattern where a small red candlestick is completely contained within the body of a larger preceding red candlestick.
 */
export const CDLHOMINGPIGEON = 'CDLHOMINGPIGEON' as const;

/**
 * Identical Three Crows.
 * A highly bearish reversal pattern consisting of three consecutive long-bodied red candles, where each candle opens at or near the previous candle's close.
 */
export const CDLIDENTICAL3CROWS = 'CDLIDENTICAL3CROWS' as const;

/**
 * In-Neck Pattern.
 * A bearish continuation pattern where a red candle is followed by a green candle that opens lower but closes just inside the previous candle's close.
 */
export const CDLINNECK = 'CDLINNECK' as const;

/**
 * Inverted Hammer.
 * A bullish candlestick reversal pattern featuring a small lower body and a long upper shadow that occurs in a downtrend.
 */
export const CDLINVERTEDHAMMER = 'CDLINVERTEDHAMMER' as const;

/**
 * Kicking.
 * A reliable two-candle reversal pattern consisting of a marubozu followed by a gap and another marubozu moving in the opposite direction.
 */
export const CDLKICKING = 'CDLKICKING' as const;

/**
 * Kicking - bull/bear determined by the longer marubozu.
 * A variation of the Kicking pattern where the dominant direction is defined by the relative length of the bullish or bearish marubozu.
 */
export const CDLKICKINGBYLENGTH = 'CDLKICKINGBYLENGTH' as const;

/**
 * Ladder Bottom.
 * A five-candle bullish reversal pattern starting with three consecutive red candles with lower closes, followed by a candle with an upper shadow, and completed by a long green candle.
 */
export const CDLLADDERBOTTOM = 'CDLLADDERBOTTOM' as const;

/**
 * Long Legged Doji.
 * A doji candlestick with long upper and lower shadows, signaling extreme indecision and a struggle between buyers and sellers.
 */
export const CDLLONGLEGGEDDOJI = 'CDLLONGLEGGEDDOJI' as const;

/**
 * Long Line Candle.
 * A candlestick with an unusually large real body, indicating intense buying or selling interest during the period.
 */
export const CDLLONGLINE = 'CDLLONGLINE' as const;

/**
 * Marubozu.
 * A candlestick with a long body and little to no shadow, signaling that one side dominated the market from the open to the close.
 */
export const CDLMARUBOZU = 'CDLMARUBOZU' as const;

/**
 * Matching Low.
 * A two-candle bullish reversal pattern where two consecutive red candles close at the exact same price level.
 */
export const CDLMATCHINGLOW = 'CDLMATCHINGLOW' as const;

/**
 * Mat Hold.
 * A highly reliable five-candle bullish trend continuation pattern featuring a strong green candle, a gap up followed by three small down candles, and a final surging green candle.
 */
export const CDLMATHOLD = 'CDLMATHOLD' as const;

/**
 * Morning Doji Star.
 * A bullish three-candle reversal pattern consisting of a long red candle, a gapped doji, and a long green candle confirming the turn upward.
 */
export const CDLMORNINGDOJISTAR = 'CDLMORNINGDOJISTAR' as const;

/**
 * Morning Star.
 * A bullish three-candle reversal pattern consisting of a long red candle, a small-bodied star candle, and a long green candle confirming the turn upward.
 */
export const CDLMORNINGSTAR = 'CDLMORNINGSTAR' as const;

/**
 * On-Neck Pattern.
 * A bearish continuation pattern where a red candle is followed by a green candle that opens lower but closes exactly at the previous candle's low.
 */
export const CDLONNECK = 'CDLONNECK' as const;

/**
 * Piercing Pattern.
 * A bullish two-candle reversal pattern where a green candle opens below the previous red candle's low but closes above its midpoint.
 */
export const CDLPIERCING = 'CDLPIERCING' as const;

/**
 * Rickshaw Man.
 * A specific long-legged doji candlestick where the real body is located exactly at or very close to the center of the trading range.
 */
export const CDLRICKSHAWMAN = 'CDLRICKSHAWMAN' as const;

/**
 * Rising/Falling Three Methods.
 * A five-candle trend continuation pattern where a strong trend candle is paused by three small-bodied counter-trend candles before resuming in the original direction.
 */
export const CDLRISEFALL3METHODS = 'CDLRISEFALL3METHODS' as const;

/**
 * Separating Lines.
 * A two-candle trend continuation pattern where the second candle opens at the same opening price as the previous counter-trend candle but moves in the opposite direction.
 */
export const CDLSEPARATINGLINES = 'CDLSEPARATINGLINES' as const;

/**
 * Shooting Star.
 * A bearish reversal candlestick pattern featuring a small lower body and a long upper shadow that occurs in an uptrend.
 */
export const CDLSHOOTINGSTAR = 'CDLSHOOTINGSTAR' as const;

/**
 * Short Line Candle.
 * A candlestick with an unusually small real body and short shadows, representing low trading activity and low volatility.
 */
export const CDLSHORTLINE = 'CDLSHORTLINE' as const;

/**
 * Spinning Top.
 * A candlestick pattern with a small real body and long, equal upper and lower shadows, signifying market indecision.
 */
export const CDLSPINNINGTOP = 'CDLSPINNINGTOP' as const;

/**
 * Stalled Pattern.
 * A bearish warning pattern consisting of three consecutive green candles where momentum stalls on the final candle, indicated by a small real body.
 */
export const CDLSTALLEDPATTERN = 'CDLSTALLEDPATTERN' as const;

/**
 * Stick Sandwich.
 * A three-candle pattern where a green candle is sandwiched between two red candles that share the same closing price, indicating a potential support bottom.
 */
export const CDLSTICKSANDWICH = 'CDLSTICKSANDWICH' as const;

/**
 * Takuri (Dragonfly Doji with very long lower shadow).
 * A specialized dragonfly doji candlestick featuring an exceptionally long lower shadow, indicating powerful bullish absorption of selling pressure near a bottom.
 */
export const CDLTAKURI = 'CDLTAKURI' as const;

/**
 * Tasuki Gap.
 * A three-candle trend continuation pattern where a gap in the trend direction is followed by a counter-trend candle that opens inside the gap but fails to fully close it.
 */
export const CDLTASUKIGAP = 'CDLTASUKIGAP' as const;

/**
 * Thrusting Pattern.
 * A bearish continuation pattern where a green candle opens below the previous red candle's low and closes near, but below, its midpoint.
 */
export const CDLTHRUSTING = 'CDLTHRUSTING' as const;

/**
 * Tristar Pattern.
 * A highly significant three-candle reversal pattern consisting of three consecutive doji candles.
 */
export const CDLTRISTAR = 'CDLTRISTAR' as const;

/**
 * Unique 3 River.
 * A rare bullish three-candle bottom reversal pattern featuring a long red candle, an inside red candle with a long lower shadow, and a small green candle.
 */
export const CDLUNIQUE3RIVER = 'CDLUNIQUE3RIVER' as const;

/**
 * Upside Gap Two Crows.
 * A bearish three-candle reversal pattern where a gap up is followed by two consecutive red candles, with the second engulfing the first.
 */
export const CDLUPSIDEGAP2CROWS = 'CDLUPSIDEGAP2CROWS' as const;

/**
 * Upside/Downside Gap Three Methods.
 * A trend continuation pattern where a gap in the direction of the trend is completely filled by a large counter-trend candle on the third period.
 */
export const CDLXSIDEGAP3METHODS = 'CDLXSIDEGAP3METHODS' as const;

/**
 * Chande Momentum Oscillator.
 * A momentum indicator that calculates the difference between sum of up days and down days divided by total price movement to show overbought/oversold levels.
 */
export const CMO = 'CMO' as const;

/**
 * Pearson's Correlation Coefficient ®.
 * Measures the linear correlation between two distinct price data series over a specified lookback period.
 */
export const CORREL = 'CORREL' as const;

/**
 * Double Exponential Moving Average.
 * A fast-reacting moving average that combines a single EMA and double EMA to reduce lag compared to traditional moving averages.
 */
export const DEMA = 'DEMA' as const;

/**
 * Directional Movement Index.
 * Measures trend strength and direction by analyzing the relationships between the high, low, and close prices of consecutive periods.
 */
export const DX = 'DX' as const;

/**
 * Exponential Moving Average.
 * A type of moving average that places a greater weight and significance on the most recent data points to react quickly to price changes.
 */
export const EMA = 'EMA' as const;

/**
 * Hilbert Transform - Dominant Cycle Period.
 * Estimates the current dominant cycle period (in bars) of a market's cyclic fluctuations.
 */
export const HT_DCPERIOD = 'HT_DCPERIOD' as const;

/**
 * Hilbert Transform - Dominant Cycle Phase.
 * Calculates the current phase (from 0 to 360 degrees) of the dominant market cycle.
 */
export const HT_DCPHASE = 'HT_DCPHASE' as const;

/**
 * Hilbert Transform - Phasor Components.
 * Generates the In-phase and Quadrature-phase components of the input price series to analyze cycles.
 */
export const HT_PHASOR = 'HT_PHASOR' as const;

/**
 * Hilbert Transform - SineWave.
 * Plots two curves (Sine and Lead-Sine) representing the cyclic phase of the market, helping to identify cycles and turning points.
 */
export const HT_SINE = 'HT_SINE' as const;

/**
 * Hilbert Transform - Instantaneous Trendline.
 * Removes dominant cycles from the price data to show a smooth, lag-free trendline representing the underlying direction.
 */
export const HT_TRENDLINE = 'HT_TRENDLINE' as const;

/**
 * Hilbert Transform - Trend vs Cycle Mode.
 * Identifies whether the current market environment is dominated by a clear trend or a cyclic trading range.
 */
export const HT_TRENDMODE = 'HT_TRENDMODE' as const;

/**
 * Kaufman Adaptive Moving Average.
 * An intelligent moving average that adjusts its sensitivity based on market noise and volatility (speeding up during trends, slowing down in choppy ranges).
 */
export const KAMA = 'KAMA' as const;

/**
 * Linear Regression.
 * Plots the endpoint of a linear regression line drawn over a specified period to estimate current fair value.
 */
export const LINEARREG = 'LINEARREG' as const;

/**
 * Linear Regression Angle.
 * Measures the angle of a linear regression line to quantify the current velocity and slope of a trend.
 */
export const LINEARREG_ANGLE = 'LINEARREG_ANGLE' as const;

/**
 * Linear Regression Intercept.
 * Calculates the starting intercept value of a linear regression line calculated over a specified period.
 */
export const LINEARREG_INTERCEPT = 'LINEARREG_INTERCEPT' as const;

/**
 * Linear Regression Slope.
 * Measures the change in price per bar of a linear regression line to define trend strength and direction.
 */
export const LINEARREG_SLOPE = 'LINEARREG_SLOPE' as const;

/**
 * Moving Average.
 * A generic moving average wrapper that can be configured to calculate various types of moving averages (SMA, EMA, WMA, etc.).
 */
export const MA = 'MA' as const;

/**
 * Moving Average Convergence/Divergence.
 * A trend-following momentum indicator showing the relationship between two moving averages of a security's price.
 */
export const MACD = 'MACD' as const;

/**
 * MACD with controllable MA type.
 * An extended version of the standard MACD indicator where the user can customize the mathematical type of moving average used.
 */
export const MACDEXT = 'MACDEXT' as const;

/**
 * Moving Average Convergence/Divergence Fix 12/26.
 * The standard MACD calculation hardcoded to use a 12-period fast EMA and 26-period slow EMA.
 */
export const MACDFIX = 'MACDFIX' as const;

/**
 * MESA Adaptive Moving Average.
 * A highly adaptive moving average that utilizes Hilbert Transform cycle phase to adjust its speed, locking onto price levels without overshoot.
 */
export const MAMA = 'MAMA' as const;

/**
 * Highest value over a specified period.
 * Scans a lookback window and returns the maximum price or value encountered.
 */
export const MAX = 'MAX' as const;

/**
 * Index of highest value over a specified period.
 * Scans a lookback window and returns the relative index/position of the maximum price value.
 */
export const MAXINDEX = 'MAXINDEX' as const;

/**
 * Median Price.
 * Calculated as the simple average of the High and Low prices for a given period.
 */
export const MEDPRICE = 'MEDPRICE' as const;

/**
 * Money Flow Index.
 * A volume-weighted version of the Relative Strength Index (RSI) that measures buying and selling pressure.
 */
export const MFI = 'MFI' as const;

/**
 * MidPoint over period.
 * Calculates the middle value between the highest high and lowest low within a specified lookback period.
 */
export const MIDPOINT = 'MIDPOINT' as const;

/**
 * Midpoint Price over period.
 * Calculates the middle value between the highest high price and lowest low price within a specified lookback period.
 */
export const MIDPRICE = 'MIDPRICE' as const;

/**
 * Lowest value over a specified period.
 * Scans a lookback window and returns the minimum price or value encountered.
 */
export const MIN = 'MIN' as const;

/**
 * Index of lowest value over a specified period.
 * Scans a lookback window and returns the relative index/position of the minimum price value.
 */
export const MININDEX = 'MININDEX' as const;

/**
 * Lowest and highest values over a specified period.
 * Scans a lookback window and returns both the minimum and maximum values encountered.
 */
export const MINMAX = 'MINMAX' as const;

/**
 * Indexes of lowest and highest values over a specified period.
 * Scans a lookback window and returns the relative index/position of both the minimum and maximum values.
 */
export const MINMAXINDEX = 'MINMAXINDEX' as const;

/**
 * Minus Directional Indicator.
 * Measures the presence and strength of downward price movement over a specified period.
 */
export const MINUS_DI = 'MINUS_DI' as const;

/**
 * Minus Directional Movement.
 * Measures the raw downward price movement between consecutive period ranges.
 */
export const MINUS_DM = 'MINUS_DM' as const;

/**
 * Momentum.
 * Measures the change in price of an asset over a specified period, indicating the speed of price movement.
 */
export const MOM = 'MOM' as const;

/**
 * Normalized Average True Range.
 * Normalizes the ATR value as a percentage of close price, allowing comparison of volatility across assets with different price scales.
 */
export const NATR = 'NATR' as const;

/**
 * On Balance Volume.
 * A cumulative trading volume indicator that relates volume flow to price change to predict future price moves.
 */
export const OBV = 'OBV' as const;

/**
 * Plus Directional Indicator.
 * Measures the presence and strength of upward price movement over a specified period.
 */
export const PLUS_DI = 'PLUS_DI' as const;

/**
 * Plus Directional Movement.
 * Measures the raw upward price movement between consecutive period ranges.
 */
export const PLUS_DM = 'PLUS_DM' as const;

/**
 * Percentage Price Oscillator.
 * A momentum indicator that measures the difference between two moving averages as a percentage rather than an absolute value.
 */
export const PPO = 'PPO' as const;

/**
 * Rate of change : ((price/prevPrice)-1)*100.
 * Calculates the percentage difference between the current price and a historical price from a specified number of periods ago.
 */
export const ROC = 'ROC' as const;

/**
 * Rate of change Percentage: (price-prevPrice)/prevPrice.
 * Calculates the raw percentage rate of change of prices over a lookback window.
 */
export const ROCP = 'ROCP' as const;

/**
 * Rate of change ratio: (price/prevPrice).
 * Calculates the raw ratio of the current price relative to a historical price.
 */
export const ROCR = 'ROCR' as const;

/**
 * Rate of change ratio 100 scale: (price/prevPrice)*100.
 * Calculates the ratio of the current price relative to a historical price on a 100-scale basis.
 */
export const ROCR100 = 'ROCR100' as const;

/**
 * Relative Strength Index.
 * A popular momentum oscillator that measures the speed and change of price movements between 0 and 100 to identify overbought/oversold states.
 */
export const RSI = 'RSI' as const;

/**
 * Parabolic SAR.
 * A trend-following indicator designed to find potential trend reversals and place trailing stop-loss levels.
 */
export const SAR = 'SAR' as const;

/**
 * Parabolic SAR - Extended.
 * An advanced formulation of the Parabolic SAR allowing custom acceleration factors for both long and short positions.
 */
export const SAREXT = 'SAREXT' as const;

/**
 * Simple Moving Average.
 * Calculates the unweighted mean of a specified number of recent data points.
 */
export const SMA = 'SMA' as const;

/**
 * Standard Deviation.
 * Measures the dispersion of price data relative to its mean, serving as a fundamental metric of market volatility.
 */
export const STDDEV = 'STDDEV' as const;

/**
 * Stochastic.
 * A momentum indicator comparing a closing price to its price range over a given period, consisting of %K and %D lines.
 */
export const STOCH = 'STOCH' as const;

/**
 * Stochastic Fast.
 * The raw, unsmoothed version of the Stochastic indicator designed for high sensitivity to short-term momentum changes.
 */
export const STOCHF = 'STOCHF' as const;

/**
 * Stochastic Relative Strength Index.
 * Applies the Stochastic formula to RSI values instead of raw price data, highlighting extreme overbought and oversold conditions.
 */
export const STOCHRSI = 'STOCHRSI' as const;

/**
 * Summation.
 * Calculates the sum of all price or data values over a specified lookback period.
 */
export const SUM = 'SUM' as const;

/**
 * Triple Exponential Moving Average (T3).
 * An advanced moving average that applies triple smoothing to reduce lag and suppress noise, utilizing a custom damping volume factor.
 */
export const T3 = 'T3' as const;

/**
 * Triple Exponential Moving Average.
 * A fast, lag-reduced moving average created by combining three separate exponential moving averages.
 */
export const TEMA = 'TEMA' as const;

/**
 * True Range.
 * Calculates the absolute greatest value among: current high minus current low, current high minus previous close, or current low minus previous close.
 */
export const TRANGE = 'TRANGE' as const;

/**
 * Triangular Moving Average.
 * A double-smoothed moving average that places the greatest weight on the middle portion of the lookback window.
 */
export const TRIMA = 'TRIMA' as const;

/**
 * 1-day Rate-Of-Change (ROC) of a Triple Smooth EMA.
 * An oscillator that filters out short-term price noise to spot long-term momentum shifts and crossovers.
 */
export const TRIX = 'TRIX' as const;

/**
 * Time Series Forecast.
 * Uses linear regression equations to forecast what an asset price should be in the immediate next period.
 */
export const TSF = 'TSF' as const;

/**
 * Typical Price.
 * Calculated as the simple average of the High, Low, and Close prices for a given period.
 */
export const TYPPRICE = 'TYPPRICE' as const;

/**
 * Ultimate Oscillator.
 * A momentum oscillator that combines three different timeframes to minimize false divergence signals.
 */
export const ULTOSC = 'ULTOSC' as const;

/**
 * Variance.
 * Measures how far a set of price values are spread out from their average value.
 */
export const VAR = 'VAR' as const;

/**
 * Weighted Close Price.
 * Calculated as the average of the High, Low, and double-weighted Close prices for a given period.
 */
export const WCLPRICE = 'WCLPRICE' as const;

/**
 * Williams' %R.
 * A momentum indicator that measures overbought and oversold levels on a scale of -100 to 0, comparing close price to highest highs.
 */
export const WILLR = 'WILLR' as const;

/**
 * Weighted Moving Average.
 * A moving average that assigns linearly descending weights to historical data points, prioritizing recent price action.
 */
export const WMA = 'WMA' as const;
