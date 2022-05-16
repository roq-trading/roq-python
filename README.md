# WORK IN PROGRESS !!!

This is the Python binding of the Roq C++ API.


### Using


This is how to install Roq's Python binding

```bash
conda install -y --channel https://roq-trading.com/conda/stable \
    roq-python
```

Samples can be found [here](https://github.com/roq-trading/roq-python/tree/master/samples).

Many of the examples benefit greatly from `@typedispatch` decorator from
[fastcore](https://github.com/fastai/fastcore) which you can install like this

```bash
conda install -y fastcore
```

This is what you need to write a subscriber

```python
class Subscriber(roq.client.Handler):
    def __init__(self, *args):
        roq.client.Handler.__init__(self)  # required by pybind11

    @typedispatch
    @classmethod
    def callback(
        cls,
        message_info: roq.MessageInfo,
        top_of_book: roq.TopOfBook,
    ):
        bp, bq, ap, aq = top_of_book.layer.astuple()
        mid = (bp * aq + ap * bq) / (bq + aq)
        print(f"mid={mid:.2f}")


roq.client.set_flags(  # currently required to deal with flags
    dict(
        name="trader",
        timer_freq="1s",
    )
)

config = roq.client.Config(
    symbols={
        "deribit": {"BTC-PERPETUAL"},
    },
)

connections = ["{HOME}/run/deribit-test.sock".format(**os.environ)]

manager = roq.client.Manager(Subscriber, config, connections)

while manager.dispatch():
    pass
```


## License

The project is released under the terms of the BSD 3-Clause license.
