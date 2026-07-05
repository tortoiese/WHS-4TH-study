function memscn(module, pattern) {
    var matches = Memory.scanSync(module.base, module.size, pattern)
    return matches.length > 0 ? matches[0].address : null
}

function is_all_uppercase(str) {
    return str != "" && str.replaceAll(/[A-Z]+/g, '') === ""
}

function int2hex_pattern(num) {
    var res = []
    for(var i = 0; i < 4; i++) {
        res.push((num & 0xff).toString(16).padStart(2, "0"))
        num >>= 8
    }
    return res.join(" ")
}

function str2hex_pattern(str) {
    var res = []
    for(var i = 0; i < str.length; i++) {
        res.push(str.charCodeAt(i).toString(16).padStart(2, "0"))
    }
    return res.join(" ")
}

function get_packet_function(m) {
    var encrypt = null
    var decrypt = null

    // TODO: implement to get raw packet bytes
    // hint: use memscn

    return {
        encrypt: encrypt,
        decrypt: decrypt
    }
}

const m = Process.enumerateModules()[0]

// [TODO] loco_send pattern scan
const functions = get_packet_function(m)
const loco_send = functions.encrypt
const loco_recv = functions.decrypt

function str2ab(str) {
    let buf = new ArrayBuffer(str.length);
    let bufView = new Uint8Array(buf);
    for (var i=0, strLen=str.length; i < strLen; i++) {
        bufView[i] = str.charCodeAt(i);
    }
    return buf;
}

Interceptor.attach(loco_send, {
    onEnter(args) {
        const pkt = args[1].readPointer()
        const method = pkt.add(6).readCString()
        var bson = pkt.add(22)
        const size = 0 // FIXME
        const data = null // FIXME

        if(!is_all_uppercase(method))
            return

        console.log(`[SEND::${method}]`)
        console.log(hexdump(data))
    }
})

Interceptor.attach(loco_recv, {
    onEnter(args) {
        this.buf = args[2].readPointer().sub(22)
    },
    onLeave(ret) {
        const res = this.buf
        const method = res.add(6).readCString()
        const bson = res.add(22)
        const size = 0 // FIXME
        const data = null // FIXME

        if (!is_all_uppercase(method))
            return

        console.log(`[RECV::${method}]`)
        console.log(hexdump(data))
    }
})