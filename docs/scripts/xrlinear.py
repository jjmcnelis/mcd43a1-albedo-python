import numpy as np
import xarray as xr
from scipy import stats


def xr_linear(xarr): 
    """Linear regression over xarray.DataArray time dimension."""
    
    # getting shapes
    m = np.prod(xarr.shape[1:]).squeeze()
    n = xarr.shape[0]
    
    # create x and y variables
    x = xarr.time.to_pandas().index.to_julian_date().values[:, None]
    y = xarr.to_masked_array().reshape(n, -1)
    
    # regression
    xm = x.mean(0)  # mean
    ym = y.mean(0)  # mean
    ya = y - ym  # anomaly
    xa = x - xm  # anomaly
    
    # variance and covariances
    xss = (xa ** 2).sum(0) / (n - 1)  # variance of x (with df as n-1)
    yss = (ya ** 2).sum(0) / (n - 1)  # variance of y (with df as n-1)
    xys = (xa * ya).sum(0) / (n - 1)  # covariance (with df as n-1)
    
    # slope and intercept
    slope = xys / xss
    intercept = ym - (slope * xm)
    
    # statistics about fit
    df = n - 2
    r = xys / (xss * yss)**0.5
    t = r * (df / ((1 - r) * (1 + r)))**0.5
    p = stats.distributions.t.sf(abs(t), df)
    out = xarr[:2].mean('time')
    
    # first create variable for slope and adjust meta   
    xarr_slope = out.copy()
    xarr_slope.name += '_slope'
    xarr_slope.attrs['units'] = 'units / day'
    xarr_slope.values = slope.reshape(xarr.shape[1:])
    
    # do the same for the p value
    xarr_p = out.copy()
    xarr_p.name += '_Pvalue'
    xarr_p.attrs['info'] = ("If p < 0.05 then the results from"
                            " 'slope' are significant.")
    xarr_p.values = p.reshape(xarr.shape[1:])
    xarr_out = xarr_slope.to_dataset(name='slope')
    xarr_out['pval'] = xarr_p

    return(xarr_out)