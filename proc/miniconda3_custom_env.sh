### Getting pysolar
git clone https://github.com/pingswept/pysolar.git
mv pysolar proc




### Optionally, install jupyter extensions for convenience


conda install -c conda-forge jupyter_nbextensions_configurator
conda install -c conda-forge jupyter_contrib_nbextensions

jupyter nbextension install --user https://rawgithub.com/minrk/ipython_extensions/master/nbextensions/toc.js
curl -L https://rawgithub.com/minrk/ipython_extensions/master/nbextensions/toc.css > $(jupyter --data-dir)/nbextensions/toc.css
jupyter nbextension enable toc
