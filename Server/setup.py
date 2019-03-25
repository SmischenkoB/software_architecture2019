from setuptools import setup

requires = [
    'deform',
    'pyramid',
    'pyramid_chameleon',
    'pyramid_tm',
    'sqlalchemy',
    'waitress',
    'zope.sqlalchemy',
    'pymysql',
    'requests',
    'jsonpickle',
    'redis',
    'beautifulsoup4'
]

setup(
    name='services',
    author='',
    install_requires=requires,
    entry_points={
        'paste.app_factory': [
            'main = services:main'
        ],
        'console_scripts': [
            'initialize_service_db = services.initialize_db:main'
        ]
    }
)
