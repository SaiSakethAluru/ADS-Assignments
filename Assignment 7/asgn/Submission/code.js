const Web3 = require('web3');
const { Contract } = require('web3-eth-contract');
const web3 = new Web3(new Web3.providers.HttpProvider("http://localhost:7545"));
const Tx = require('ethereumjs-tx').Transaction


let addr1 = '0xBdc3F6D4Af2a227a76d803fF41DECd006A03bf65';
let privateKey1 = Buffer.from('ca0a871f7c30cbd74f8bf2dc02f171619febc1b5bd6903d3de339d9e45c4e0a2', 'hex');

let contractAddr = "0xDAda88e141BC68F16bfD167790e378e7Cbd697F4";

let contractABI = [
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "uint16[5]",
          "name": "_subjects",
          "type": "uint16[5]"
        }
      ],
      "name": "setsubjects",
      "outputs": [],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": false,
      "inputs": [
        {
          "internalType": "uint16[5]",
          "name": "_credits",
          "type": "uint16[5]"
        }
      ],
      "name": "setcredits",
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
          "name": "subjectId",
          "type": "uint256"
        },
        {
          "internalType": "string",
          "name": "_grade",
          "type": "string"
        }
      ],
      "name": "setgrade",
      "outputs": [],
      "payable": false,
      "stateMutability": "nonpayable",
      "type": "function"
    },
    {
      "constant": true,
      "inputs": [],
      "name": "calculateSGPA",
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
  ];

let contract = new web3.eth.Contract(contractABI, contractAddr); 


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

// web3.eth.getTransactionCount("0x7197184A07E0bbEA642d73a8a725C7b28676CC89").then(console.log);   

sendTx(0, contractAddr, contract.methods.setsubjects([60003,60016,60010,60042,47006]));
sendTx(1, contractAddr, contract.methods.setcredits([4,3,3,3,6]));
sendTx(2, contractAddr, contract.methods.setgrade(0,"E"));
sendTx(3, contractAddr, contract.methods.setgrade(1,"E"));
sendTx(4, contractAddr, contract.methods.setgrade(2,"E"));
sendTx(5, contractAddr, contract.methods.setgrade(3,"E"));
sendTx(6, contractAddr, contract.methods.setgrade(4,"E"));

contract.methods.calculateSGPA().call((err,res) => {
    console.log('SGPA: '+res);
})