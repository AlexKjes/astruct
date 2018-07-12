from distutils.core import setup, Extension


#BinaryTreeNode = Extension('BinaryTreeNode', sources = ['astruct.c'])


setup (name = 'astruct',
       version = '0.1',
       description = 'Alexanders (soon to be) awesome datastructure package!',
       ext_modules = [
		Extension('astruct', sources = ['astruct.c'])#, 'btn.c'])
	])
