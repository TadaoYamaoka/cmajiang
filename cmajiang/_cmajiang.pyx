from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool

cdef extern from "cmajiang.h":
    cdef cppclass __Shoupai:
        __Shoupai() except +
        __Shoupai(const vector[string]& qipai) except +
        __Shoupai(const string& paistr) except +
        __Shoupai(const __Shoupai& shoupai) except +

        string toString()

        # 自摸
        void zimo(const string& p, bool check) except +
        # 打牌
        void dapai(const string& p, bool check) except +
        # 副露
        void fulou(const string& m, bool check) except +
        # 杠(槓)
        void gang(const string& m, bool check) except +
        # 门前(門前)
        bool menqian()
        # 立直
        bool lizhi()
        # 打牌取得
        vector[string] get_dapai(bool check) except +
        # 吃(チー)面子取得
        vector[string] get_chi_mianzi(const string& p, bool check) except +
        # 碰(ポン)面子取得
        vector[string] get_peng_mianzi(const string& p) except +
        # 杠(槓)面子取得
        vector[string] get_gang_mianzi(const string& p) except +
        vector[string] get_gang_mianzi() except +

        vector[int] bingpai(const string& s)

        vector[int] m()
        vector[int] p()
        vector[int] s()
        vector[int] z()
        int m(const int n)
        int p(const int n)
        int s(const int n)
        int z(const int n)
        vector[string] fulou()
        string zimo()

cdef class Shoupai:
    """手牌"""

    cdef __Shoupai __shoupai

    def __cinit__(self, str paistr=None, list qipai=None, Shoupai shoupai=None):
        cdef string paistr_b
        cdef vector[string] qipai_b
        if paistr:
            paistr_b = paistr.encode("ascii")
            self.__shoupai = __Shoupai(paistr_b)
        elif qipai is not None:
            for p in qipai:
                qipai_b.push_back(p.encode("ascii"))
            self.__shoupai = __Shoupai(qipai_b)
        elif shoupai is not None:
            self.__shoupai = __Shoupai(shoupai.__shoupai)
        else:
            self.__shoupai = __Shoupai()

    def __copy__(self):
        return Shoupai(shoupai=self)

    def copy(self):
        """Creates a copy of the current shoupai.

        :return: A new Shoupai object with the same state.
        """
        return Shoupai(shoupai=self)

    def __repr__(self):
        return self.__shoupai.toString().decode("ascii")

    def zimo(self, str p, bool check=True):
        # 自摸
        self.__shoupai.zimo(p.encode("ascii"), check)
        return self

    def dapai(self, str p, bool check=True):
        # 打牌
        self.__shoupai.dapai(p.encode("ascii"), check)
        return self

    def fulou(self, str m, bool check=True):
        # 副露
        self.__shoupai.fulou(m.encode("ascii"), check)
        return self

    def gang(self, str m, bool check=True):
        # 杠(槓)
        self.__shoupai.gang(m.encode("ascii"), check)
        return self

    @property
    def menqian(self):
        # 门前(門前)
        return self.__shoupai.menqian()

    @property
    def lizhi(self):
        # 立直
        return self.__shoupai.lizhi()

    def get_dapai(self, bool check=True):
        # 打牌取得
        return [p.decode("ascii") for p in self.__shoupai.get_dapai(check)]

    def get_chi_mianzi(self, str p, bool check=True):
        # 吃(チー)面子取得
        return [m.decode("ascii") for m in self.__shoupai.get_chi_mianzi(p.encode("ascii"), check)]

    def get_peng_mianzi(self, str p):
        # 碰(ポン)面子取得
        return [m.decode("ascii") for m in self.__shoupai.get_peng_mianzi(p.encode("ascii"))]

    def get_gang_mianzi(self, str p=None):
        # 杠(槓)面子取得
        if p is None:
            return [m.decode("ascii") for m in self.__shoupai.get_gang_mianzi()]
        else:
            return [m.decode("ascii") for m in self.__shoupai.get_gang_mianzi(p.encode("ascii"))]

    def get_bingpai(self, str s):
        return self.__shoupai.bingpai(s.encode("ascii"))

    @property
    def m(self):
        return self.__shoupai.m()

    @property
    def p(self):
        return self.__shoupai.p()

    @property
    def s(self):
        return self.__shoupai.s()

    @property
    def z(self):
        return self.__shoupai.z()

    def get_m(self, int n):
        return self.__shoupai.m(n)

    def get_p(self, int n):
        return self.__shoupai.p(n)

    def get_s(self, int n):
        return self.__shoupai.s(n)

    def get_z(self, int n):
        return self.__shoupai.z(n)

    def get_fulou(self):
        return [m.decode("ascii") for m in self.__shoupai.fulou()]

    def get_zimo(self):
        return self.__shoupai.zimo().decode("ascii")
