from setuptools import find_packages, setup

package_name = 'py_pkg'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='nikhil',
    maintainer_email='200414nikhiljagdale@gmail.com',
    description='TODO: Package description',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            #syntax: <executable_name> = <package_name>.<node_name>:<function_to_start>:
            "my_node = py_pkg.first_node:main"         #ros2 takes this file and makes some modification to it so that it becomes executable for ros2 and it will be installed in the install folder of the ros2 workspace
        ],
    },
)
