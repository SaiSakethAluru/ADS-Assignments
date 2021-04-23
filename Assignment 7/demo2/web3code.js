const Web3 = require('web3');
const { Contract } = require('web3-eth-contract');
const web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:7545"));
const Tx = require('ethereumjs-tx').Transaction


let addr1 = '0x0c6b9C4B598E1FBE8E1b43058Aa33d61C36E64C4';
let privateKey1 = Buffer.from('df030b3c82dfbf38c6351f23cdea8b8c0517a3677ec5a713743f9124191b4231', 'hex');

let contractAddr = "0x124016d7aE21a6ADa0e86F97cD5200aA80174800"
let contractABI = [
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "string",
          "name": "_name",
          "type": "string"
        }
      ],
      "name": "setName",
      "outputs": [],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "uint256",
          "name": "_roll",
          "type": "uint256"
        }
      ],
      "name": "setRoll",
      "outputs": [],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "string",
          "name": "_hall",
          "type": "string"
        }
      ],
      "name": "setHall",
      "outputs": [],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "getName",
      "outputs": [
        {
          "internalType": "string",
          "name": "",
          "type": "string"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "getRoll",
      "outputs": [
        {
          "internalType": "uint256",
          "name": "",
          "type": "uint256"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "getHall",
      "outputs": [
        {
          "internalType": "string",
          "name": "",
          "type": "string"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "helloWorld",
      "outputs": [
        {
          "internalType": "string",
          "name": "",
          "type": "string"
        }
      ],
      "payable": false,
      "stateMutability": "pure",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "getABCD",
      "outputs": [
        {
          "internalType": "uint256",
          "name": "",
          "type": "uint256"
        }
      ],
      "payable": false,
      "stateMutability": "view",
      "type": "function"
    }
  ]

let contract1 = new web3.eth.Contract(contractABI, contractAddr); 


function sendTx(_nonce, _to, _data) {
    let txObject = {
        nonce : web3.utils.toHex(_nonce),
        gasLimit : web3.utils.toHex(800000),
        gasPrice : web3.utils.toHex(web3.utils.toWei('20', 'gwei')),
        to : _to,
        data : _data.encodeABI()
    }

    let tx = new Tx(txObject);
    tx.sign(privateKey1);

    let serializedTx = tx.serialize();
    let raw = '0x' + serializedTx.toString('hex');

    web3.eth.sendSignedTransaction(raw, (err, txHash) => {
        console.log('error :', err);
        console.log('txHash :', txHash);
    });
}

web3.eth.getTransactionCount("0xCC81A0C9cf7cC304719e0f673a71f31379fE3B32").then(console.log);
sendTx(0, contractAddr, contract1.methods.setName("ritik"));
sendTx(1, contractAddr, contract1.methods.setRoll(12));

contract1.methods.getABCD().call((err, res) => {
    console.log(res);
})


const abiDecoder = require('abi-decoder');
abiDecoder.addABI(contractABI)

web3.eth.getTransaction("0xc0452d743794a5b603d9d0838b51b21d01a5fc90d972fd477503eb0e2ec93eb1", (err, res) => {
    var decodedData = abiDecoder.decodeMethod(res.input)
    console.log(decodedData)
});



