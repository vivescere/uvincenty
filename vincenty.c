#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <Python.h>

#define PI 3.14159265358979323846264338327950288
#define degreesToRadians(angleDegrees) ((angleDegrees) * PI / 180.0)

double vincenty(double lat1, double lon1, double lat2, double lon2) {
    double a = 6378137;
    double f = 1.0 / 298.257223563;
    double b = 6356752.314245;

    int MAX_ITERATIONS = 200;
    double CONVERGENCE_THRESHOLD = 1e-12;

    if (lat1 == lat2 && lon1 == lon2)
        return 0.0;

    double U1 = atan((1.0 - f) * tan(degreesToRadians(lat1)));
    double U2 = atan((1.0 - f) * tan(degreesToRadians(lat2)));

    double L = degreesToRadians(lon2 - lon1);
    double Lambda = L;

    double sinU1 = sin(U1);
    double cosU1 = cos(U1);
    double sinU2 = sin(U2);
    double cosU2 = cos(U2);

    double cosSqAlpha, sinSigma, cos2SigmaM, cosSigma, sigma;
    double sinLambda, cosLambda, sinAlpha;
    double C, LambdaPrev;

    bool converged = false;

    for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
        sinLambda = sin(Lambda);
        cosLambda = cos(Lambda);

        sinSigma = sqrt(
            pow((cosU2 * sinLambda), 2) +
            pow(cosU1 * sinU2 - sinU1 * cosU2 * cosLambda, 2)
        );

        if (sinSigma == 0)
            return 0.0;

        cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
        sigma = atan2(sinSigma, cosSigma);
        sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
        cosSqAlpha = 1.0 - pow(sinAlpha, 2.0);

        if (cosSqAlpha != 0)
            cos2SigmaM = cosSigma - 2.0 * sinU1 * sinU2 / cosSqAlpha;
        else
            cos2SigmaM = 0;

        C = f / 16.0 * cosSqAlpha * (4.0 + f * (4.0 - 3.0 * cosSqAlpha));
        LambdaPrev = Lambda;

        Lambda = L + (1.0 - C) * f * sinAlpha * (sigma + C * sinSigma *
                                               (cos2SigmaM + C * cosSigma *
                                                (-1.0 + 2.0 * pow(cos2SigmaM, 2))));

        if (fabs(Lambda - LambdaPrev) < CONVERGENCE_THRESHOLD) {
            converged = true;
            break;
        }
    }

    if (!converged)
        return -1;

    double uSq = cosSqAlpha * (pow(a, 2) - pow(b, 2)) / pow(b, 2);
    double A = 1.0 + uSq / 16384.0 * (4096.0 + uSq * (-768.0 + uSq * (320.0 - 175.0 * uSq)));
    double B = uSq / 1024.0 * (256.0 + uSq * (-128.0 + uSq * (74.0 - 47.0 * uSq)));

    double deltaSigma = B * sinSigma * (
        cos2SigmaM + B / 4.0 * (
            cosSigma *
                 (-1.0 + 2.0 * pow(cos2SigmaM, 2)) -
                 B / 6.0 * cos2SigmaM *
                 (-3.0 + 4.0 * pow(sinSigma, 2)) *
                 (-3.0 + 4.0 * pow(cos2SigmaM, 2))
        )
    );

    double s = b * A * (sigma - deltaSigma);
    s /= 1000.0;

    return s;
}

static PyObject* py_vincenty(PyObject *self, PyObject *args) {
    double lat1, lon1, lat2, lon2;

    if (!PyArg_ParseTuple(args, "dddd", &lat1, &lon1, &lat2, &lon2))
        return NULL;

    double result = vincenty(lat1, lon1, lat2, lon2);

    // Failure to converge
    if (result == -1)
        Py_RETURN_NONE;

    return PyFloat_FromDouble(result);
}


static PyMethodDef uvincenty_methods[] = {
    {
        "vincenty", py_vincenty, METH_VARARGS,
        "A fast implementation of the inverse vincenty formula.\nArguments: (lat1, lon1, lat2, lon2)"
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef uvincenty_definition = {
    PyModuleDef_HEAD_INIT,
    "uvincenty",
    NULL,
    -1,
    uvincenty_methods
};

PyMODINIT_FUNC PyInit_uvincenty(void) {
    Py_Initialize();
    return PyModule_Create(&uvincenty_definition);
}
