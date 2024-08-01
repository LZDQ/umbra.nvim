"""
This is sample code to showcase this colorscheme.
"""

from scipy.interpolate import interp1d
import numpy as np
import matplotlib.pyplot as plt


class ParentClass:
    def __init__(self, name):
        self.name = name
        print("Parent:", name)

class ChildClass(ParentClass):
    def name(self):
        return self.name

obj = ChildClass("John")

# breakpoint()

import asyncio

async def async_function():
    print("Asynchronous function")
    await asyncio.sleep(1)
    return "Completed"

def sync_function():
    result = asyncio.run(async_function())
    # result = async_function()
    print(result)

# sync_function()


def f2(a):
    a += np.array([1, 1])
    a = np.array([1, 0])

b = np.array([1, 2])
f2(b)
print(b)

# Number of points and random seed for reproducibility
num_points = 100
np.random.seed(0)

# Generate random points
x = np.sort(np.random.rand(num_points) * 100)
y = np.random.rand(num_points) * 100

# Create a smooth line using cubic interpolation
f = interp1d(x, y, kind='cubic')
x_smooth = np.linspace(x.min(), x.max(), 500)
y_smooth = f(x_smooth)

# Plotting
plt.plot(x_smooth, y_smooth)
plt.scatter(x, y, color='red')  # original points
plt.title("Smooth Random Line")
# plt.show()

class ParentClass:
    """
    Parent class to be inherited
    """
    def __init__(self, name):
        self.name = name
        print("Parent:", name)

class ChildClass(ParentClass):
    """
    Children class to inherit from parent class 
    """
    def name(self):
        return self.name

obj = ChildClass("John")

arr = [1, 0.2, .3, -4, 0x5, 06, 1e100]
a0 = int(arr[0])

print(f"a0 is: {a0} and array is {arr}  escape {{arr}}")

