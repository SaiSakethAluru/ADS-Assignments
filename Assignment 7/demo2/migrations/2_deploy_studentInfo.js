const contract1 = artifacts.require("StudentInfo");

module.exports = function (deployer) {
  deployer.deploy(contract1);
};
